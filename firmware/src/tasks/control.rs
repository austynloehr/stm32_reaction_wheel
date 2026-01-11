use crate::monitor_task_rate;
use common::types::{InputEvent, LedState, MotorCtrlMode, MotorRequest, OutputEvent};
use core::sync::atomic::{AtomicBool, Ordering};
use defmt::*;
use embassy_futures::join::join3;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::{Receiver, Sender};
use embassy_time::{Duration, Instant, Ticker};

#[embassy_executor::task]
pub async fn run(
    input_channel: Receiver<'static, CriticalSectionRawMutex, InputEvent, 128>,
    output_channel: Sender<'static, CriticalSectionRawMutex, OutputEvent, 128>,
) {
    const TASK_RATE: Duration = Duration::from_millis(1);

    let startup_complete = AtomicBool::new(false);

    let rx_loop = async {
        let mut motor_last = Instant::now();
        let mut imu_last = Instant::now();
        let mut imu_startup_complete: bool = false;
        let mut motor_startup_complete: bool = false;

        loop {
            let event = input_channel.receive().await;
            match event {
                InputEvent::Motor(_status) => {
                    if !motor_startup_complete {
                        motor_startup_complete = true;
                    } else {
                        let now = Instant::now();
                        let dt = now.duration_since(motor_last);

                        if dt > Duration::from_millis(11) || dt < Duration::from_millis(9) {
                            warn!("Motor Rx Jitter: {}ms", dt.as_millis());
                        }
                    }
                    motor_last = Instant::now();
                }
                InputEvent::Imu(_status) => {
                    if !imu_startup_complete {
                        imu_startup_complete = true;
                    } else {
                        let now = Instant::now();
                        let dt = now.duration_since(imu_last);

                        if dt > Duration::from_millis(11) || dt < Duration::from_millis(9) {
                            warn!("IMU Rx Jitter: {}ms", dt.as_millis());
                        }
                    }
                    imu_last = Instant::now();
                }
                InputEvent::Button(state) => {
                    debug!("Enable button: {}", state);
                }
            }

            if !startup_complete.load(Ordering::Acquire)
                && motor_startup_complete
                && imu_startup_complete
            {
                info!("Startup complete");
                startup_complete.store(true, Ordering::Release);
            }
        }
    };

    let tx_loop = async {
        let mut speed: f32 = 0.0;
        let mut step: f32 = 5.0;
        let mut ticker = Ticker::every(TASK_RATE);

        loop {
            ticker.next().await;
            monitor_task_rate!(control_monitor, TASK_RATE.as_millis(), 20, 1000);
            // Send command
            let _request =
                OutputEvent::Motor(MotorRequest::new(MotorCtrlMode::Speed, speed as i32));
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
            if startup_complete.load(Ordering::Acquire) {
                match output_channel.try_send(OutputEvent::GreenLed(LedState::On)) {
                    Ok(_) => {}
                    Err(e) => warn!("{:?}", e),
                }
                match output_channel.try_send(OutputEvent::RedLed(LedState::Off)) {
                    Ok(_) => {}
                    Err(e) => warn!("{:?}", e),
                }
            } else {
                match output_channel.try_send(OutputEvent::GreenLed(LedState::Off)) {
                    Ok(_) => {}
                    Err(e) => warn!("{:?}", e),
                }
                match output_channel.try_send(OutputEvent::RedLed(LedState::On)) {
                    Ok(_) => {}
                    Err(e) => warn!("{:?}", e),
                }
            }
            ticker.next().await;
        }
    };

    join3(rx_loop, tx_loop, led_ctrl_loop).await;
}
