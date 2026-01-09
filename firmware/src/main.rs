#![no_std]
#![no_main]

use defmt::*;
use embassy_executor::Spawner;
use firmware as _; // Link panic handler and other global setup from lib.rs
use firmware::{channels, hardware, signals, tasks};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("Starting Application...");

    // Initialize Hardware
    let hardware_interfaces = hardware::init();
    info!("Hardware initialized!");

    // Initialize Channels
    let channels = channels::TaskChannels::take().unwrap();
    info!("Channels initialized!");

    // Initialize Signals
    let signals = signals::TaskSignals::take().unwrap();
    info!("Signals initialized!");

    // Run IMU Task
    _spawner
        .spawn(tasks::imu::run(
            hardware_interfaces.imu,
            channels.input_channel.sender(),
        ))
        .unwrap();

    // Run CAN Task
    _spawner
        .spawn(tasks::can::run(
            hardware_interfaces.can,
            signals.vesc_status_tx,
            channels.can_tx_channel.receiver(),
        ))
        .unwrap();

    // Run motor task
    _spawner
        .spawn(tasks::motor::run(
            channels.input_channel.sender(),
            channels.can_tx_channel.sender(),
            signals.vesc_status_rx,
            signals.motor_request_rx,
        ))
        .unwrap();

    // Run control task
    _spawner
        .spawn(tasks::control::run(
            channels.input_channel.receiver(),
            channels.output_channel.sender(),
        ))
        .unwrap();

    // Run coms router task
    _spawner
        .spawn(tasks::coms_router::run(
            channels.output_channel.receiver(),
            signals.motor_request_tx,
            signals.green_led_tx,
            signals.red_led_tx,
        ))
        .unwrap();

    // Run LED task
    _spawner
        .spawn(tasks::leds::run(
            hardware_interfaces.green_led,
            hardware_interfaces.red_led,
            signals.green_led_rx,
            signals.red_led_rx,
        ))
        .unwrap();

    // Run button task
    _spawner
        .spawn(tasks::button::run(
            channels.input_channel.sender(),
            hardware_interfaces.enable_btn,
        ))
        .unwrap();

    info!("All tasks started!");
}
