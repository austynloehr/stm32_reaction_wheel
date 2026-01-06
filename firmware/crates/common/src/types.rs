#[derive(Debug, Clone, Copy, defmt::Format, PartialEq)]
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
    Motor(MotorRequest),
    Logger(u32),
    GreenLed(LedState),
    RedLed(LedState),
}

#[derive(Debug, Clone, Copy, defmt::Format, PartialEq)]
pub enum LedState {
    On,
    Off,
    Blink,
}

#[derive(Debug, Clone, Copy, defmt::Format, PartialEq)]
pub enum ButtonState {
    Pressed,
    Released,
}

#[derive(Debug, Clone, Copy, defmt::Format, PartialEq)]
pub enum MotorCtrlMode {
    Current,
    Speed,
}

#[derive(Debug, Clone, Copy, defmt::Format, PartialEq)]
pub struct MotorRequest {
    mode: MotorCtrlMode,
    value: i32,
}

impl MotorRequest {
    pub fn new(mode: MotorCtrlMode, value: i32) -> Self {
        Self { mode, value }
    }

    pub fn mode(&self) -> MotorCtrlMode {
        self.mode
    }

    pub fn value(&self) -> i32 {
        self.value
    }
}
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct CanFrame {
    id: u32,
    extended: bool,
    data: [u8; 8],
    dlc: u8,
}

impl CanFrame {
    pub fn new(id: u32, extended: bool, data: [u8; 8], dlc: u8) -> Self {
        Self {
            id,
            extended,
            data,
            dlc,
        }
    }

    pub fn id(&self) -> u32 {
        self.id
    }

    pub fn is_extended(&self) -> bool {
        self.extended
    }

    pub fn data(&self) -> &[u8] {
        &self.data[..self.dlc as usize]
    }

    pub fn dlc(&self) -> u8 {
        self.dlc
    }
}

impl defmt::Format for CanFrame {
    fn format(&self, fmt: defmt::Formatter) {
        defmt::write!(
            fmt,
            "CanFrame {{ id: 0x{=u32:x}, data: {=[u8]:x} }}",
            self.id,
            &self.data[..self.dlc as usize],
        )
    }
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
