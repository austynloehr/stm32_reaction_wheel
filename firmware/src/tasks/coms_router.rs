use crate::types::SignalSender;
use common::types::{MotorRequest, TxEvent};
use defmt::*;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Receiver;

#[embassy_executor::task]
pub async fn run(
    tx_channel: Receiver<'static, CriticalSectionRawMutex, TxEvent, 128>,
    motor_request_tx: SignalSender<MotorRequest>,
) {
    loop {
        let event = tx_channel.receive().await;
        match event {
            TxEvent::Motor(request) => {
                motor_request_tx.signal(request);
            }
            TxEvent::Logger(data) => {
                info!("Received log data: {:?}", data);
            }
        }
    }
}
