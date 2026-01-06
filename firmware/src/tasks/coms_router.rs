use crate::types::SignalSender;
use common::types::{LedState, MotorRequest, TxEvent};
use defmt::*;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Receiver;

#[embassy_executor::task]
pub async fn run(
    tx_channel: Receiver<'static, CriticalSectionRawMutex, TxEvent, 128>,
    motor_request_tx: SignalSender<MotorRequest>,
    green_led_tx: SignalSender<LedState>,
    red_led_tx: SignalSender<LedState>,
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
            TxEvent::GreenLed(state) => {
                green_led_tx.signal(state);
            }
            TxEvent::RedLed(state) => {
                red_led_tx.signal(state);
            }
        }
    }
}
