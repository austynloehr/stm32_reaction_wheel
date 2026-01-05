use crate::types::{CanFrame, SignalSender};
use core::sync::atomic::{AtomicBool, Ordering};
use defmt::*;
use embassy_futures::join::join;
use embassy_stm32::can::enums::BusError;
use embassy_stm32::can::frame::Header;
use embassy_stm32::can::{Can, CanRx, CanTx, Frame};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Receiver;
use embassy_time::{Duration, Timer};
use embedded_can::{ExtendedId, Id, StandardId};

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
    vesc_status_tx: SignalSender<CanFrame>,
    can_tx_channel: Receiver<'static, CriticalSectionRawMutex, CanFrame, 128>,
) {
    can.enable().await;

    let (mut tx, mut rx) = can.split();

    let bus_ok = AtomicBool::new(false);
    let mut valid_rx_cnt: u8 = 0;

    let rx_loop = async {
        loop {
            match receive(&mut rx, &vesc_status_tx).await {
                Ok(_) => {
                    // Wait for 10 valid frames before declaring the bus as OK
                    if !bus_ok.load(Ordering::Relaxed) {
                        valid_rx_cnt += 1;
                        if valid_rx_cnt >= 10 {
                            info!("CAN Bus OK, enabling transmit");
                            bus_ok.store(true, Ordering::Relaxed);
                            can_tx_channel.clear(); // Drop any messages we may have received before the bus was OK
                        }
                    }
                }
                Err(e) => error!("CAN Rx Error: {:?}", e),
            }
        }
    };

    let tx_loop = async {
        loop {
            // Wait for the bus to be OK before sending frames
            // Other devices on bus may have startup delay to ACK frames
            if bus_ok.load(Ordering::Relaxed) {
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
    vesc_status_tx: &SignalSender<CanFrame>,
) -> Result<(), BusError> {
    match rx.read().await {
        Ok(envelope) => {
            let frame: Frame = envelope.frame;
            let data_slice = frame.data();
            let dlc = data_slice.len() as u8;

            // Rx data is a slice, we need to copy it into a fixed-size array
            let mut data_buf = [0u8; 8];
            data_buf[..data_slice.len()].copy_from_slice(data_slice);

            // Rx ID is an enum, we need to extract the value
            let (id_val, is_extended) = match frame.id() {
                Id::Standard(id) => (id.as_raw() as u32, false),
                Id::Extended(id) => (id.as_raw(), true),
            };
            debug!("CAN Rx: ID={:x} Data={:?}", id_val, frame.data());

            if id_val == VESC_STATUS_ID {
                let frame: CanFrame = CanFrame::new(id_val, is_extended, data_buf, dlc);
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
    can_tx_channel: &Receiver<'static, CriticalSectionRawMutex, CanFrame, 128>,
) {
    let frame: CanFrame = can_tx_channel.receive().await;

    // Convert ourCanFrame to embassy can frame
    let id = if frame.is_extended() {
        Id::Extended(ExtendedId::new(frame.id()).unwrap())
    } else {
        Id::Standard(StandardId::new(frame.id() as u16).unwrap())
    };
    let data: &[u8] = frame.data();
    let header = Header::new(id, frame.data().len() as u8, false);
    let frame: Frame = Frame::new(header, data).unwrap();

    debug!("CAN Tx: {:?}", frame);
    tx.write(&frame).await;
}
