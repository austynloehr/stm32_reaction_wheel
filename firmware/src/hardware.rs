//! # Hardware Configuration
//!
//! This module initializes the STM32F412 hardware and provides interfaces to
//! the peripherals used in the application.
//!
//! # Usage
//!
//! ```
//! let hardware = hardware::init();
//! let imu = hardware.imu;
//! let can = hardware.can;
//! ```

use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_stm32::bind_interrupts;
use embassy_stm32::i2c::{self, I2c};
use embassy_stm32::peripherals;
use embassy_stm32::time::Hertz;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::mutex::Mutex;
use static_cell::StaticCell;

use crate::types::SharedI2c;

// Define Interrupts
bind_interrupts!(struct Irqs {
    I2C1_EV => i2c::EventInterruptHandler<peripherals::I2C1>;
    I2C1_ER => i2c::ErrorInterruptHandler<peripherals::I2C1>;
});

/// Holds handles to all initialized hardware peripherals.
pub struct SystemHardware {
    pub imu: SharedI2c,
    // Using raw peripherals for now until drivers are implemented
    pub can: peripherals::CAN1,
    pub index: peripherals::PA0, // Example GPIO
    pub uart: peripherals::USART1,
}

/// # Initialize Hardware
///
/// Initializes the MCU, clocks, and peripherals.
///
/// ## Returns
///
/// * `SystemHardware` - Struct containing fully configured peripheral handles.
pub fn init() -> SystemHardware {
    // Initialize Embassy STM32 with default config
    let p = embassy_stm32::init(Default::default());

    // --- I2C Initialization ---
    // I2C1, PB6 (SCL), PB7 (SDA), DMA1_CH1 (TX), DMA1_CH0 (RX), 400k baud
    // Configure settings
    let mut config = i2c::Config::default();
    config.frequency = Hertz::khz(400);

    // Initialize driver instance
    let i2c = I2c::new(p.I2C1, p.PB6, p.PB7, Irqs, p.DMA1_CH1, p.DMA1_CH0, config);

    // Wrap driver in mutex for shared bus
    static I2C_BUS: StaticCell<
        Mutex<CriticalSectionRawMutex, I2c<'static, embassy_stm32::mode::Async, i2c::Master>>,
    > = StaticCell::new();
    let i2c_bus = I2C_BUS.init(Mutex::new(i2c));

    // Add device to shared i2c bus
    let imu_i2c = I2cDevice::new(i2c_bus);

    // --- Return Hardware ---
    SystemHardware {
        imu: imu_i2c,
        can: *p.CAN1,
        index: *p.PA0, // Placeholder for GPIO
        uart: *p.USART1,
    }
}
