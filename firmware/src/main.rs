#![no_std]
#![no_main]

use defmt::*;
use embassy_executor::Spawner;
use firmware as _; // Link panic handler and other global setup from lib.rs
use firmware::{channels, hardware, signals, tasks};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    info!("Starting Application...");

    // Initialize Hardware Interfaces
    let h = hardware::init();
    info!("Hardware initialized");

    // Initialize Channels
    let c = channels::TaskChannels::take().unwrap();
    info!("Channels initialized");

    // Initialize Signals
    let s = signals::TaskSignals::take().unwrap();
    info!("Signals initialized");

    // Start CAN task
    _spawner
        .spawn(tasks::can::run(
            h.can,
            s.vesc_status.sender,
            c.can_tx.receiver(),
        ))
        .unwrap();

    // Start LED task
    _spawner
        .spawn(tasks::leds::run(
            h.green_led,
            h.red_led,
            s.green_led.receiver,
            s.red_led.receiver,
        ))
        .unwrap();

    // Start button task
    _spawner
        .spawn(tasks::button::run(c.inputs.sender(), h.enable_btn))
        .unwrap();

    // Start motor task
    _spawner
        .spawn(tasks::motor::run(
            c.inputs.sender(),
            c.can_tx.sender(),
            s.vesc_status.receiver,
            s.motor_request.receiver,
        ))
        .unwrap();

    // Start IMU Task
    _spawner
        .spawn(tasks::imu::run(h.imu, c.inputs.sender()))
        .unwrap();

    // Start coms router task
    _spawner
        .spawn(tasks::coms_router::run(
            c.outputs.receiver(),
            s.motor_request.sender,
            s.green_led.sender,
            s.red_led.sender,
        ))
        .unwrap();

    // Start control task
    _spawner
        .spawn(tasks::control::run(c.inputs.receiver(), c.outputs.sender()))
        .unwrap();

    info!("All tasks started");
}
