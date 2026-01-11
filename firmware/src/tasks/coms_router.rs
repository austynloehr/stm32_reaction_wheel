use crate::types::SignalSender;
use common::types::{LedState, MotorRequest, OutputEvent};
use defmt::*;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Receiver;

#[embassy_executor::task]
pub async fn run(
    receiver: Receiver<'static, CriticalSectionRawMutex, OutputEvent, 128>,
    motor_request_sender: SignalSender<MotorRequest>,
    green_led_sender: SignalSender<LedState>,
    red_led_sender: SignalSender<LedState>,
) {
    loop {
        let event = receiver.receive().await;
        match event {
            OutputEvent::Motor(request) => {
                motor_request_sender.signal(request);
            }
            OutputEvent::Logger(data) => {
                info!("Received log data: {:?}", data);
            }
            OutputEvent::GreenLed(state) => {
                green_led_sender.signal(state);
            }
            OutputEvent::RedLed(state) => {
                red_led_sender.signal(state);
            }
        }
    }
}
