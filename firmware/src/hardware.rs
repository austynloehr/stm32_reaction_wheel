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
use embassy_stm32::can::filter::ListEntry32;
use embassy_stm32::can::{self, Can, CanRx, CanTx, Fifo};
use embassy_stm32::i2c::{self, I2c};
use embassy_stm32::peripherals;
use embassy_stm32::rcc::{
    APBPrescaler, Hse, HseMode, Pll, PllMul, PllPDiv, PllPreDiv, PllQDiv, PllSource, Sysclk,
};
use embassy_stm32::time::Hertz;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::mutex::Mutex;
use embedded_can::{ExtendedId, Id};

use static_cell::StaticCell;

use crate::types::{SharedCan, SharedI2c};

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
    pub motor: SharedCan,
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
    // Setup 100MHz System Clock (16MHz HSE, P=2, Q=2)
    let mut config = embassy_stm32::Config::default();
    config.rcc.hse = Some(Hse {
        freq: Hertz(16_000_000), // HSE 16MHz (based on legacy N=100/M=8 => 100MHz Sys)
        mode: HseMode::Oscillator,
    });
    config.rcc.pll_src = PllSource::HSE;
    config.rcc.pll = Some(Pll {
        prediv: PllPreDiv::DIV8,
        mul: PllMul::MUL100,
        divp: Some(PllPDiv::DIV2), // P=2
        divq: Some(PllQDiv::DIV2), // Q=2
        divr: None,
    });
    config.rcc.sys = Sysclk::PLL1_P;
    config.rcc.apb1_pre = APBPrescaler::DIV2; // 50 MHz APB1

    // Initialize Embassy STM32 peripherals
    let p = embassy_stm32::init(config);

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

    // --- CAN Initialization ---
    // Initialize CAN driver instance (CAN1, PA11 (RX), PA12 (TX))
    let mut can = Can::new(p.CAN1, p.PA11, p.PA12, Irqs);

    // Set bitrate to 500k and accept only VESC status message
    can.set_bitrate(500_000);
    let filter = ListEntry32::data_frames_with_id(Id::Extended(ExtendedId::new(0x901).unwrap()));
    can.modify_filters()
        .enable_bank(0, Fifo::Fifo0, [filter, filter]);

    // Split into tx/rx and wrap in mutex for shared bus usage
    let (tx, rx) = can.split();
    static CAN_TX: StaticCell<Mutex<CriticalSectionRawMutex, CanTx<'static>>> = StaticCell::new();
    static CAN_RX: StaticCell<Mutex<CriticalSectionRawMutex, CanRx<'static>>> = StaticCell::new();
    let can_tx = CAN_TX.init(Mutex::new(tx));
    let can_rx = CAN_RX.init(Mutex::new(rx));

    let motor_can = SharedCan {
        tx: can_tx,
        rx: can_rx,
    };

    // index and uart removed until drivers are implemented
    SystemHardware {
        imu: imu_i2c,
        motor: motor_can,
    }
}
