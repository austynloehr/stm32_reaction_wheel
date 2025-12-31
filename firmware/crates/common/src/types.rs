use embedded_can::Frame;

#[derive(Debug, Clone, Copy, defmt::Format)]
pub struct MotorStatus {
    speed_rpm: i32,
    current_ma: i32,
}

impl MotorStatus {
    pub fn new(speed_rpm: i32, current_ma: i32) -> Self {
        Self {
            speed_rpm,
            current_ma,
        }
    }

    /// Get motor speed in RPM
    pub fn speed(&self) -> i32 {
        self.speed_rpm
    }

    /// Get motor current in mA
    pub fn current(&self) -> i32 {
        self.current_ma
    }
}

#[derive(Debug, Clone, Copy, defmt::Format)]
pub enum RxEvent {
    Imu(ImuSample),
    Motor(MotorStatus),
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

/// Async CAN trait (needed because there is no standard one in embedded-hal-async yet)
#[allow(async_fn_in_trait)]
pub trait AsyncCanHal {
    type Error: core::fmt::Debug;
    type Frame: Frame;

    /// Transmit a frame asynchronously
    async fn write(&mut self, frame: &Self::Frame) -> Result<(), Self::Error>;

    /// Receive a frame asynchronously
    async fn read(&mut self) -> Result<Self::Frame, Self::Error>;
}
