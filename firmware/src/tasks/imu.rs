use crate::types::{RxEvent, SharedI2c};
use defmt::*;
use drivers::mpu6050;
use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, channel::Sender};
use embassy_time::{Duration, Timer};

#[embassy_executor::task]
pub async fn run(i2c: SharedI2c, sender: Sender<'static, CriticalSectionRawMutex, RxEvent, 128>) {
    // Init Driver (must happen before read loop)
    let mut imu = mpu6050::Mpu6050::new(i2c).init().await.unwrap();
    info!("MPU6050 Initialized!");

    loop {
        // Read IMU data from driver
        let sample = imu.step().await.unwrap();
        info!("IMU: {} {}", sample.accel(), sample.gyro());

        // Send IMU data to rx channel queue
        let msg = RxEvent::Imu(sample);
        if sender.try_send(msg).is_err() {
            warn!("RxChannel full, dropping IMU sample");
        }

        // Sample every 10ms
        Timer::after(Duration::from_millis(10)).await;
    }
}
