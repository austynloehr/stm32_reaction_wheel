# Reaction Wheel Firmware

Rust/Embassy implementation of Reaction Wheel controls on STM32F412.

## Organization

### Folders
- `src/`: Application source code
- `src/behavior/`: Modules containing core system behavior
- `src/drivers/`: Component drivers (e.g. MPU6050, VESC)
- `src/tasks/`: Async tasks (e.g. IMU polling, VESC RX/TX, Control)

### Modules
- `hardware.rs`: Core initialization code for MCU (clocks, peripherals, interrupts)
- `channels.rs`: Creates static async channels for inter-task communication.
- `types.rs`: Data types shared between multiple modules.
- `macros.rs`: Helper macros.

## Build & Run

```bash
cargo check
cargo build
cargo run
```
