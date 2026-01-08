use crate::types::SignalSender;
use core::sync::atomic::{AtomicBool, Ordering};
use defmt::*;
use embassy_futures::join::join;
use embassy_stm32::can::enums::BusError;
use embassy_stm32::can::{Can, CanRx, CanTx, Frame};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Receiver;
use embassy_time::{Duration, Timer};

const VESC_STATUS_ID: u32 = 0x901;

/// CAN Task
///
/// This task handles all CAN communication.
/// It splits the CAN peripheral into a transmitter and receiver,
/// and runs two loops in parallel:
///
/// 1. rx_loop: Receives frames from the CAN bus and forwards frames to their respective signals.
/// 2. tx_loop: Sends frames from the can_tx_channel to the CAN bus.
#[embassy_executor::task]
pub async fn run(
    mut can: Can<'static>,
    vesc_status_tx: SignalSender<Frame>,
    can_tx_channel: Receiver<'static, CriticalSectionRawMutex, Frame, 128>,
) {
    can.enable().await;

    let (mut tx, mut rx) = can.split();

    let bus_ok = AtomicBool::new(false);
    let mut consecutive_rx_valid: u8 = 0;
    let mut consecutive_rx_errors: u8 = 0;
    const MAX_CONSECUTIVE_ERRORS: u8 = 50;

    let rx_loop = async {
        loop {
            match receive(&mut rx, &vesc_status_tx).await {
                Ok(_) => {
                    // Reset error counter on successful receive
                    consecutive_rx_errors = 0;

                    // Wait for 10 consecutive valid frames before declaring the bus as OK
                    if !bus_ok.load(Ordering::Acquire) {
                        consecutive_rx_valid += 1;
                        if consecutive_rx_valid >= 10 {
                            info!("CAN Bus OK, enabling transmit");
                            bus_ok.store(true, Ordering::Release);
                            can_tx_channel.clear(); // Drop any messages we may have received before the bus was OK
                            consecutive_rx_valid = 0; // Reset for next recovery cycle
                        }
                    }
                }
                Err(e) => {
                    consecutive_rx_errors += 1;

                    // Reset valid counter - we need consecutive good messages
                    consecutive_rx_valid = 0;

                    // If bus was previously OK, reset it after errors accumulate
                    if consecutive_rx_errors >= 5 && bus_ok.load(Ordering::Acquire) {
                        warn!("CAN Bus experiencing errors, disabling transmit");
                        bus_ok.store(false, Ordering::Release);
                    }

                    error!(
                        "CAN Rx Error: {:?} (consecutive: {})",
                        e, consecutive_rx_errors
                    );

                    // If we hit max consecutive errors, enter recovery mode
                    if consecutive_rx_errors >= MAX_CONSECUTIVE_ERRORS {
                        error!(
                            "CAN bus failure: {} consecutive errors. Entering recovery mode...",
                            consecutive_rx_errors
                        );

                        // Disable bus
                        bus_ok.store(false, Ordering::Release);

                        // Wait 10 seconds for bus to stabilize
                        Timer::after(Duration::from_secs(10)).await;

                        // Reset counters and try again
                        consecutive_rx_errors = 0;
                        consecutive_rx_valid = 0;

                        info!("CAN recovery complete, attempting to resume receive operations");
                        continue;
                    }

                    // Add small delay on error to avoid hammering the bus
                    Timer::after(Duration::from_millis(10)).await;
                }
            }
        }
    };

    let tx_loop = async {
        loop {
            // Wait for the bus to be OK before sending frames
            // Other devices on bus may have startup delay to ACK frames
            if bus_ok.load(Ordering::Acquire) {
                write(&mut tx, &can_tx_channel).await;
            } else {
                Timer::after(Duration::from_millis(10)).await; // Wait and try again
            }
        }
    };

    join(rx_loop, tx_loop).await;
}

/// Wait to receive a frame from the CAN bus.
///
/// If the frame is a vesc status frame, forward it to the vesc_status_tx signal.
async fn receive(
    rx: &mut CanRx<'static>,
    vesc_status_tx: &SignalSender<Frame>,
) -> Result<(), BusError> {
    match rx.read().await {
        Ok(envelope) => {
            let frame: Frame = envelope.frame;

            // Extract ID from the frame
            let id_val = match frame.id() {
                embedded_can::Id::Standard(id) => id.as_raw() as u32,
                embedded_can::Id::Extended(id) => id.as_raw(),
            };
            debug!("CAN Rx: ID={:x} Data={:?}", id_val, frame.data());

            if id_val == VESC_STATUS_ID {
                vesc_status_tx.signal(frame);
            }

            Ok(())
        }
        Err(e) => Err(e),
    }
}

/// Write all frames in the CAN Tx channel to the bus
async fn write(
    tx: &mut CanTx<'static>,
    can_tx_channel: &Receiver<'static, CriticalSectionRawMutex, Frame, 128>,
) {
    let frame: Frame = can_tx_channel.receive().await;

    debug!("CAN Tx: {:?}", frame);
    tx.write(&frame).await;
}
