use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_stm32::i2c::{self, I2c};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;

pub type SharedI2c = I2cDevice<
    'static,
    CriticalSectionRawMutex,
    I2c<'static, embassy_stm32::mode::Async, i2c::Master>,
>;

#[derive(Debug, Clone, Copy, defmt::Format)]
pub enum RxEvent {
    Imu(ImuSample),
}

#[derive(Debug, Clone, Copy, defmt::Format)]
pub enum TxEvent {
    Motor(u32),
    Logger(u32),
}

crate::define_3d_mux!(
    RawAccelVector,
    "Strongly typed wrapper for raw accelerometer data."
);
crate::define_3d_mux!(
    RawGyroVector,
    "Strongly typed wrapper for raw gyroscope data."
);

/// Single message containing synchronized imu data
#[derive(Debug, Clone, Copy, defmt::Format)]
pub struct ImuSample {
    accel: RawAccelVector,
    gyro: RawGyroVector,
}

impl ImuSample {
    pub fn new(accel: RawAccelVector, gyro: RawGyroVector) -> Self {
        Self { accel, gyro }
    }

    pub const ZERO: Self = Self {
        accel: RawAccelVector::new(0.0, 0.0, 0.0),
        gyro: RawGyroVector::new(0.0, 0.0, 0.0),
    };

    // Getters provide Read-Only access

    pub fn accel(&self) -> RawAccelVector {
        self.accel
    }

    pub fn gyro(&self) -> RawGyroVector {
        self.gyro
    }
}
