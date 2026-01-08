use crate::types::SignalReceiver;
use common::types::LedState;
use drivers::led::Led;
use embassy_futures::join::join;
use embassy_futures::select::{Either, select};
use embassy_stm32::gpio::Output;
use embassy_sync::blocking_mutex::raw::NoopRawMutex;
use embassy_sync::mutex::Mutex;
use embassy_time::{Duration, Ticker};

#[embassy_executor::task]
#[allow(clippy::manual_is_multiple_of)]
pub async fn run(
    green_led_pin: Output<'static>,
    red_led_pin: Output<'static>,
    green_led_rx: SignalReceiver<LedState>,
    red_led_rx: SignalReceiver<LedState>,
) {
    let mut green_led = Led::new(green_led_pin, true, false).unwrap_or_else(|e| {
        defmt::panic!("Failed to initialize green LED: {:?}", e);
    });
    let mut red_led = Led::new(red_led_pin, true, false).unwrap_or_else(|e| {
        defmt::panic!("Failed to initialize red LED: {:?}", e);
    });

    let green_led_state: Mutex<NoopRawMutex, LedState> = Mutex::new(LedState::Off);
    let red_led_state: Mutex<NoopRawMutex, LedState> = Mutex::new(LedState::Off);

    let request_loop = async {
        loop {
            match select(green_led_rx.wait(), red_led_rx.wait()).await {
                Either::First(request) => {
                    let mut state = green_led_state.lock().await;
                    *state = request;
                }
                Either::Second(request) => {
                    let mut state = red_led_state.lock().await;
                    *state = request;
                }
            }
        }
    };

    let update_loop = async {
        const POLLING_INTERVAL: Duration = Duration::from_millis(50);
        const BLINK_DIVIDER: u32 = 10; // 50ms * 10 = 500ms blink toggle

        let mut ticker = Ticker::every(POLLING_INTERVAL);
        let mut ticks: u32 = 0;

        loop {
            // Get the latest state
            let (green_cmd, red_cmd) = {
                let g = green_led_state.lock().await;
                let r = red_led_state.lock().await;
                (*g, *r)
            };

            ticks = ticks.wrapping_add(1);
            let should_toggle = ticks % BLINK_DIVIDER == 0;

            match green_cmd {
                LedState::Off => {
                    let _ = green_led.off();
                }
                LedState::On => {
                    let _ = green_led.on();
                }
                LedState::Blink => {
                    if should_toggle {
                        let _ = green_led.toggle();
                    }
                }
            }

            match red_cmd {
                LedState::Off => {
                    let _ = red_led.off();
                }
                LedState::On => {
                    let _ = red_led.on();
                }
                LedState::Blink => {
                    if should_toggle {
                        let _ = red_led.toggle();
                    }
                }
            }

            ticker.next().await;
        }
    };
    join(request_loop, update_loop).await;
}
