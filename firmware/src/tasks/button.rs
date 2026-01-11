use common::types::{ButtonState, InputEvent};
use defmt::*;
use embassy_futures::join::join;
use embassy_stm32::exti::ExtiInput;
use embassy_sync::blocking_mutex::raw::{CriticalSectionRawMutex, NoopRawMutex};
use embassy_sync::channel::Sender;
use embassy_sync::mutex::Mutex;
use embassy_time::{Duration, Ticker, Timer};

#[embassy_executor::task]
pub async fn run(
    sender: Sender<'static, CriticalSectionRawMutex, InputEvent, 128>,
    mut enable_btn: ExtiInput<'static>,
) {
    const DEBOUNCE_TIME: Duration = Duration::from_millis(10);
    const TRANSMIT_RATE: Duration = Duration::from_millis(100);
    let button_state: Mutex<NoopRawMutex, ButtonState> = Mutex::new(ButtonState::Released);

    let read_loop = async {
        loop {
            enable_btn.wait_for_any_edge().await;
            Timer::after(DEBOUNCE_TIME).await;

            let new_state = if enable_btn.is_low() {
                ButtonState::Pressed
            } else {
                ButtonState::Released
            };

            let mut guard = button_state.lock().await;
            if *guard != new_state {
                *guard = new_state;
                match new_state {
                    ButtonState::Pressed => debug!("Enable button pressed"),
                    ButtonState::Released => debug!("Enable button released"),
                }
            }
        }
    };

    let output_loop = async {
        let mut ticker = Ticker::every(TRANSMIT_RATE);
        loop {
            ticker.next().await;
            let state = button_state.lock().await;
            sender.send(InputEvent::Button(*state)).await;
        }
    };

    join(read_loop, output_loop).await;
}
