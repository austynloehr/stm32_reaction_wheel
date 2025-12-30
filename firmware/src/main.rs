#![no_std]
#![no_main]

use defmt::*;
use embassy_executor::Spawner;
use firmware as _; // Link panic handler and other global setup from lib.rs
use firmware::{channels, hardware, tasks};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("Hello World!");

    // Initialize Hardware
    let hardware_interfaces = hardware::init();

    // Initialize Channels
    let channels = channels::init();

    // Run IMU Task
    _spawner
        .spawn(tasks::imu::run(
            hardware_interfaces.imu,
            channels.input_channel.sender(),
        ))
        .unwrap();
}
