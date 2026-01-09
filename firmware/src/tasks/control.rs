use crate::monitor_task_rate;
use common::types::{LedState, MotorCtrlMode, MotorRequest, RxEvent, TxEvent};
use defmt::*;
use embassy_futures::join::join3;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::{Receiver, Sender};
use embassy_time::{Duration, Instant, Ticker};

#[embassy_executor::task]
pub async fn run(
    rx_channel: Receiver<'static, CriticalSectionRawMutex, RxEvent, 128>,
    tx_channel: Sender<'static, CriticalSectionRawMutex, TxEvent, 128>,
) {
    const TASK_RATE: Duration = Duration::from_millis(1);

    let rx_loop = async {
        let mut motor_last = Instant::now();
        let mut imu_last = Instant::now();
        let mut imu_start: bool = false;
        let mut motor_start: bool = false;

        loop {
            let event = rx_channel.receive().await;
            match event {
                RxEvent::Motor(_status) => {
                    if !motor_start {
                        motor_start = true;
                    } else {
                        let now = Instant::now();
                        let dt = now.duration_since(motor_last);

                        if dt > Duration::from_millis(11) || dt < Duration::from_millis(9) {
                            warn!("Motor Rx Jitter: {}ms", dt.as_millis());
                        }
                    }
                    motor_last = Instant::now();
                }
                RxEvent::Imu(_status) => {
                    if !imu_start {
                        imu_start = true;
                    } else {
                        let now = Instant::now();
                        let dt = now.duration_since(imu_last);

                        if dt > Duration::from_millis(11) || dt < Duration::from_millis(9) {
                            warn!("IMU Rx Jitter: {}ms", dt.as_millis());
                        }
                    }
                    imu_last = Instant::now();
                }
                RxEvent::Button(state) => {
                    debug!("Enable button: {}", state);
                }
            }
        }
    };

    let tx_loop = async {
        let mut speed: f32 = 0.0;
        let mut step: f32 = 5.0;
        let mut ticker = Ticker::every(TASK_RATE);

        loop {
            ticker.next().await;
            monitor_task_rate!(control_monitor, TASK_RATE.as_millis(), 20, 500);
            // Send command
            let _request = TxEvent::Motor(MotorRequest::new(MotorCtrlMode::Speed, speed as i32));
            // match tx_channel.try_send(request) {
            //     Ok(_) => {}
            //     Err(e) => warn!("{:?}", e),
            // }

            // Update speed
            speed += step;
            if speed >= 1500.0 {
                speed = 1500.0;
                step = -0.5;
            } else if speed <= -1500.0 {
                speed = -1500.0;
                step = 0.5;
            }
        }
    };

    let led_ctrl_loop = async {
        let mut ticker = Ticker::every(Duration::from_millis(1000));
        loop {
            match tx_channel.try_send(TxEvent::GreenLed(LedState::On)) {
                Ok(_) => {}
                Err(e) => warn!("{:?}", e),
            }
            match tx_channel.try_send(TxEvent::RedLed(LedState::Off)) {
                Ok(_) => {}
                Err(e) => warn!("{:?}", e),
            }
            ticker.next().await;
        }
    };

    join3(rx_loop, tx_loop, led_ctrl_loop).await;
}
