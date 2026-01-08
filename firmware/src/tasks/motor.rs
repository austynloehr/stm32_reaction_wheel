use crate::types::SignalReceiver;
use common::types::{MotorRequest, RxEvent};
use defmt::*;
use drivers::vesc::Vesc;
use embassy_futures::join::join3;
use embassy_stm32::can::Frame;
use embassy_sync::blocking_mutex::raw::{CriticalSectionRawMutex, NoopRawMutex};
use embassy_sync::channel::Sender;
use embassy_sync::mutex::Mutex;
use embassy_time::{Duration, Instant, Ticker};

#[embassy_executor::task]
pub async fn run(
    rx_channel: Sender<'static, CriticalSectionRawMutex, RxEvent, 128>,
    can_tx_channel: Sender<'static, CriticalSectionRawMutex, Frame, 128>,
    vesc_status_rx: SignalReceiver<Frame>,
    motor_request_rx: SignalReceiver<MotorRequest>,
) {
    const CMD_TX_INTERVAL: Duration = Duration::from_millis(10);
    const CAN_TIMEOUT: Duration = Duration::from_millis(1000);
    let vesc = Vesc::new();

    // Pre-create fail safe frame
    let (fail_safe_id, fail_safe_data, fail_safe_len) = vesc.create_fail_safe();
    let fail_safe_frame = match Frame::new_extended(fail_safe_id, &fail_safe_data[..fail_safe_len])
    {
        Ok(frame) => frame,
        Err(e) => {
            defmt::panic!("Failed to create fail safe frame: {:?}", e);
        }
    };

    let last_cmd_frame: Mutex<NoopRawMutex, Option<(Frame, Instant)>> = Mutex::new(None);

    let can_rx_loop = async {
        // 1. Wait until we receive a status message on CAN
        // 2. Unpack the status message
        // 3. Send the status message to the rx_channel
        // 4. If unpacking fails, log the error
        loop {
            let frame = vesc_status_rx.wait().await;
            debug!("Received VESC status message: {:?}", frame);
            if let Ok(status_msg) = vesc.unpack_status(frame.data()) {
                match rx_channel.try_send(RxEvent::Motor(status_msg)) {
                    Ok(_) => {}
                    Err(e) => debug!("{:?}", e),
                }
            }
        }
    };

    let motor_request_loop = async {
        // 1. Wait until we receive a motor request
        // 2. Create a command frame
        // 3. Store the command frame and timestamp in last_cmd_frame using a mutex
        loop {
            let request = motor_request_rx.wait().await;
            let (id, data, len) = vesc.create_command(request.mode(), request.value());
            let cmd_frame = match Frame::new_extended(id, &data[..len]) {
                Ok(frame) => frame,
                Err(e) => {
                    defmt::error!("Failed to create command frame: {:?}", e);
                    fail_safe_frame
                }
            };
            {
                let mut guard = last_cmd_frame.lock().await;
                *guard = Some((cmd_frame, Instant::now()));
            }
        }
    };

    let can_tx_loop = async {
        // 1. Wait until the CMD_TX_INTERVAL has passed
        // 2. Get the last_cmd_frame using a mutex
        // 3. Check if the last_cmd_frame is valid
        // 3a. If the last_cmd_frame is valid, send it to the can_tx_channel
        // 3b. If the last_cmd_frame is not valid, send a fail-safe frame to the can_tx_channel
        // 4. If sending fails, log the error
        let mut ticker = Ticker::every(CMD_TX_INTERVAL);
        loop {
            ticker.next().await;

            // Scope the lock so we don't hold it during the sleep
            let frame_to_send = {
                let guard = last_cmd_frame.lock().await;
                match *guard {
                    Some((frame, timestamp)) => {
                        if Instant::now().duration_since(timestamp) < CAN_TIMEOUT {
                            Some(frame)
                        } else {
                            Some(fail_safe_frame)
                        }
                    }
                    None => Some(fail_safe_frame),
                }
            };

            if let Some(frame) = frame_to_send {
                match can_tx_channel.try_send(frame) {
                    Ok(_) => {}
                    Err(e) => debug!("{:?}", e),
                }
            }
        }
    };

    join3(can_rx_loop, motor_request_loop, can_tx_loop).await;
}
