use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_stm32::i2c::{self, I2c};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;

pub type SharedI2c = I2cDevice<
    'static,
    CriticalSectionRawMutex,
    I2c<'static, embassy_stm32::mode::Async, i2c::Master>,
>;

// Re-export shared types from common crate
pub use common::types::{ImuSample, RawAccelVector, RawGyroVector, RxEvent, TxEvent};
