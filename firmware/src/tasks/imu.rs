use crate::{
    drivers::mpu6050,
    types::{RxEvent, SharedI2c},
};
use defmt::*;
use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, channel::Sender};
use embassy_time::{Duration, Timer};

#[embassy_executor::task]
pub async fn run(
    mut i2c: SharedI2c,
    sender: Sender<'static, CriticalSectionRawMutex, RxEvent, 128>,
) {
    // Init Driver (must happen before read loop)
    mpu6050::init(&mut i2c).await.unwrap();
    info!("MPU6050 Initialized!");

    loop {
        // Read IMU data from driver
        let sample = mpu6050::step(&mut i2c).await.unwrap();
        info!("IMU: {} {}", sample.accel(), sample.gyro());

        // Send IMU data to rx channel queue
        let msg = RxEvent::Imu(sample);
        if let Err(_) = sender.try_send(msg) {
            warn!("RxChannel full, dropping IMU sample");
        }

        // Sample every 10ms
        Timer::after(Duration::from_millis(10)).await;
    }
}
