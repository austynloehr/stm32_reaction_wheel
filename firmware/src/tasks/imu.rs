use crate::types::{RxEvent, SharedI2c};
use defmt::*;
use drivers::mpu6050;
use embassy_sync::{blocking_mutex::raw::CriticalSectionRawMutex, channel::Sender};
use embassy_time::{Duration, Ticker, Timer};

#[embassy_executor::task]
pub async fn run(i2c: SharedI2c, sender: Sender<'static, CriticalSectionRawMutex, RxEvent, 128>) {
    const MAX_ERROR_COUNT: u8 = 3;
    const SAMPLE_RATE: Duration = Duration::from_millis(10);

    // IMU needs time to init on startup
    Timer::after(Duration::from_millis(1000)).await;
    let mut imu = match mpu6050::Mpu6050::new(i2c).init().await {
        Ok(imu) => imu,
        Err(e) => {
            defmt::panic!("IMU Init Error: {:?}", e);
        }
    };
    info!("IMU Initialized!");

    let mut error_count: u8 = 0;
    let mut ticker = Ticker::every(SAMPLE_RATE);
    loop {
        ticker.next().await;

        // Read IMU data
        match imu.read().await {
            Ok(sample) => {
                debug!("IMU: {} {}", sample.accel(), sample.gyro());

                // Send IMU data to rx channel queue
                if let Err(e) = sender.try_send(RxEvent::Imu(sample)) {
                    debug!("{:?}", e);
                }

                // Allow some self-healing
                error_count = error_count.saturating_sub(1);
            }
            Err(e) => {
                // Allow some errors before panicking
                error!("IMU Sample Error: {:?}", e);
                error_count += 1;
                if error_count >= MAX_ERROR_COUNT {
                    defmt::panic!("IMU read error count exceeded: {:?}", e);
                }
            }
        }
    }
}
