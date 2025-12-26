#![no_std]
#![no_main]

use defmt::*;
use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_executor::Spawner;
use embassy_stm32::i2c::I2c;
use embassy_stm32::time::Hertz;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Channel;
use embassy_sync::mutex::Mutex;
use embassy_time::{Duration, Timer};
use firmware as _; // Link panic handler and other global setup from lib.rs

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("Hello World!");

    let _p = embassy_stm32::init(Default::default());

    loop {
        let tick = embassy_time::Instant::now().as_millis();
        info!("Tick: {}", tick);
        Timer::after(Duration::from_millis(1000)).await;
    }
}
