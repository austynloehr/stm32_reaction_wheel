use crate::types::{RxEvent, SharedI2c};
use defmt::*;
use drivers::mpu6050;
use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, channel::Sender};
use embassy_time::{Duration, Timer};

#[embassy_executor::task]
pub async fn run(i2c: SharedI2c, sender: Sender<'static, CriticalSectionRawMutex, RxEvent, 128>) {
    const MAX_ERROR_COUNT: u8 = 3;
    const SAMPLE_RATE: u32 = 10; // ms

    let mut imu = mpu6050::Mpu6050::new(i2c).init().await.unwrap();
    info!("MPU6050 Initialized!");

    let mut error_count: u8 = 0;
    loop {
        // Read IMU data
        match imu.read().await {
            Ok(sample) => {
                debug!("IMU: {} {}", sample.accel(), sample.gyro());

                // Send IMU data to rx channel queue
                if let Err(_e) = sender.try_send(RxEvent::Imu(sample)) {
                    debug!("RxChannel Full");
                }

                // Allow some self-healing
                if error_count > 0 {
                    error_count -= 1;
                }
            }
            Err(_e) => {
                // Allow some errors before panicking
                error!("IMU Read Error");
                error_count += 1;
                if error_count >= MAX_ERROR_COUNT {
                    defmt::panic!("IMU read error count exceeded");
                }
            }
        }
        Timer::after(Duration::from_millis(SAMPLE_RATE as u64)).await;
    }
}
