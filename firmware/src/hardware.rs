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
//! let motor = hardware.motor;
//! ```

use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_stm32::bind_interrupts;
use embassy_stm32::can::filter::Mask32;
use embassy_stm32::can::{self, Can, Fifo};
use embassy_stm32::gpio::{Input, Level, Output, Pull, Speed};
use embassy_stm32::i2c::{self, I2c};
use embassy_stm32::peripherals;
use embassy_stm32::rcc::{
    APBPrescaler, Hse, HseMode, Pll, PllMul, PllPDiv, PllPreDiv, PllQDiv, PllSource, Sysclk,
};
use embassy_stm32::time::Hertz;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::mutex::Mutex;
use static_cell::StaticCell;

use crate::types::SharedI2c;

// Define Interrupts
bind_interrupts!(struct Irqs {
    I2C1_EV => i2c::EventInterruptHandler<peripherals::I2C1>;
    I2C1_ER => i2c::ErrorInterruptHandler<peripherals::I2C1>;
    CAN1_TX => can::TxInterruptHandler<peripherals::CAN1>;
    CAN1_RX0 => can::Rx0InterruptHandler<peripherals::CAN1>;
    CAN1_RX1 => can::Rx1InterruptHandler<peripherals::CAN1>;
    CAN1_SCE => can::SceInterruptHandler<peripherals::CAN1>;
});

/// Holds handles to all initialized hardware peripherals.
pub struct SystemHardware {
    pub imu: SharedI2c,
    pub can: Can<'static>,
    pub green_led: Output<'static>,
    pub red_led: Output<'static>,
    pub enable_button: Input<'static>,
}

/// # Initialize Hardware
///
/// Initializes the MCU, clocks, and peripherals.
///
/// ## Returns
///
/// * `SystemHardware` - Struct containing fully configured peripheral handles.
pub fn init() -> SystemHardware {
    // --- Clock Setup ---
    // Setup 100MHz System Clock
    let mut config = embassy_stm32::Config::default();
    config.rcc.hse = Some(Hse {
        freq: Hertz(16_000_000), // HSE 16MHz
        mode: HseMode::Oscillator,
    });
    config.rcc.pll_src = PllSource::HSE;
    config.rcc.pll = Some(Pll {
        prediv: PllPreDiv::DIV8,
        mul: PllMul::MUL100,
        divp: Some(PllPDiv::DIV2),
        divq: Some(PllQDiv::DIV2),
        divr: None,
    });
    config.rcc.sys = Sysclk::PLL1_P;
    config.rcc.apb1_pre = APBPrescaler::DIV2;

    let p = embassy_stm32::init(config);

    // --- I2C Initialization ---
    let mut config = i2c::Config::default();
    config.frequency = Hertz::khz(400);
    let i2c = I2c::new(p.I2C1, p.PB6, p.PB7, Irqs, p.DMA1_CH1, p.DMA1_CH0, config);

    // Wrap driver in mutex for shared bus
    static I2C_BUS: StaticCell<
        Mutex<CriticalSectionRawMutex, I2c<'static, embassy_stm32::mode::Async, i2c::Master>>,
    > = StaticCell::new();
    let i2c_bus = I2C_BUS.init(Mutex::new(i2c));

    // Create device instances
    let imu = I2cDevice::new(i2c_bus);

    // --- CAN Initialization ---
    let mut can = Can::new(p.CAN1, p.PA11, p.PA12, Irqs);
    can.modify_filters()
        .enable_bank(0, Fifo::Fifo0, Mask32::accept_all());
    can.set_bitrate(500_000);
    can.set_automatic_wakeup(true);

    // GPIO Initialization
    let green_led = Output::new(p.PA7, Level::Low, Speed::Low);
    let red_led = Output::new(p.PB0, Level::Low, Speed::Low);
    let enable_button = Input::new(p.PA4, Pull::None);

    SystemHardware {
        imu,
        can,
        green_led,
        red_led,
        enable_button,
    }
}
