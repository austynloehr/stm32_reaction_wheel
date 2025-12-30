//! # MPU6050 Driver
//!
//! This module provides a driver for the MPU6050 accelerometer and gyroscope.
//!
//! # Usage
//!
//! ```
//! use crate::drivers::mpu6050;
//!
//! let mut i2c = I2c::new(...);
//! mpu6050::init(&mut i2c).await.unwrap();
//!
//! let sample = mpu6050::step(&mut i2c).await.unwrap();
//! ```
//!
//! ## Access IMU data:
//! ```
//! let accel = sample.accel();
//! let ax = accel.data().x;
//! let ay = accel.data().y;
//! let az = accel.data().z;
//!
//! let gyro = sample.gyro();
//! let gx = gyro.data().x;
//! let gy = gyro.data().y;
//! let gz = gyro.data().z;
//! ```
use crate::types::*;

#[derive(Debug, Clone, Copy)]
#[repr(u8)]
#[allow(dead_code)]
enum AfsSel {
    G2 = 0x00,  // +-2g
    G4 = 0x01,  // +-4g
    G8 = 0x02,  // +-8g
    G16 = 0x03, // +-16g
}

impl AfsSel {
    fn sensitivity(&self) -> f32 {
        match self {
            Self::G2 => 16384.0,
            Self::G4 => 8192.0,
            Self::G8 => 4096.0,
            Self::G16 => 2048.0,
        }
    }
}

#[derive(Debug, Clone, Copy)]
#[repr(u8)]
#[allow(dead_code)]
enum FsSel {
    W250 = 0x00,  // +-250 deg/s
    W500 = 0x01,  // +-500 deg/s
    W1000 = 0x02, // +-1000 deg/s
    W2000 = 0x03, // +-2000 deg/s
}

impl FsSel {
    fn sensitivity(&self) -> f32 {
        match self {
            Self::W250 => 131.0,
            Self::W500 => 65.5,
            Self::W1000 => 32.8,
            Self::W2000 => 16.4,
        }
    }
}

const MPU6050_ADDR: u8 = 0x68;
const AFS_SEL: AfsSel = AfsSel::G8;
const FS_SEL: FsSel = FsSel::W2000;

/// # MPU6050 Initialization
///
/// Wake up and configure MPU6050 for application.
///
/// # Arguments
///
/// * `i2c` - I2C device to use for communication
///
/// # Returns
///
/// * `Result<(), I::Error>` - Result of the initialization
pub async fn init<I>(i2c: &mut I) -> Result<(), I::Error>
where
    I: embedded_hal_async::i2c::I2c,
{
    const ACCEL_CONFIG_REG: u8 = 0x1C;
    const GYRO_CONFIG_REG: u8 = 0x1B;
    const PWR_MGMT1_REG: u8 = 0x6B;
    const SMPRT_DIV_REG: u8 = 0x19;
    const DLPF_REG: u8 = 0x1A;
    const SMPLRT_DIV: u8 = 0x07; // Sample Rate [kHz] = 8 / (1 + SMPLRT_DIV)
    const DLPF_CFG: u8 = 0x00;

    // Wake up (Clear sleep bit)
    i2c.write(MPU6050_ADDR, &[PWR_MGMT1_REG, 0x00]).await?;

    // Config Sample Rate Divider
    i2c.write(MPU6050_ADDR, &[SMPRT_DIV_REG, SMPLRT_DIV])
        .await?;

    // Config DLPF
    i2c.write(MPU6050_ADDR, &[DLPF_REG, DLPF_CFG]).await?;

    // Config Accelerometer Range
    i2c.write(MPU6050_ADDR, &[ACCEL_CONFIG_REG, (AFS_SEL as u8) << 3])
        .await?;

    // Config Gyroscope Range
    i2c.write(MPU6050_ADDR, &[GYRO_CONFIG_REG, (FS_SEL as u8) << 3])
        .await?;

    Ok(())
}

/// # MPU6050 Step
///
/// Read IMU data from MPU6050.
///
/// # Arguments
///
/// * `i2c` - I2C device to use for communication
///
/// # Returns
///
/// * `Result<ImuSample, I::Error>` - Result containing the IMU sample (accel and gyro data) or an error if the read fails.
pub async fn step<I>(i2c: &mut I) -> Result<ImuSample, I::Error>
where
    I: embedded_hal_async::i2c::I2c,
{
    const ACCEL_OUT_START_REG: u8 = 0x3B;
    const GYRO_OUT_START_REG: u8 = 0x43;
    const G: f32 = 9.81;

    // Buffer for accel and gyro data
    let mut accel_buf = [0u8; 6];
    let mut gyro_buf = [0u8; 6];

    // Read accel and gyro data
    // Return error if read fails
    i2c.write_read(MPU6050_ADDR, &[ACCEL_OUT_START_REG], &mut accel_buf)
        .await?;
    i2c.write_read(MPU6050_ADDR, &[GYRO_OUT_START_REG], &mut gyro_buf)
        .await?;

    // Get conversion factors
    let accel_scale = AFS_SEL.sensitivity();
    let gyro_scale = FS_SEL.sensitivity();

    // Convert raw accel data to m/s^2
    let accel_x = (i16::from_be_bytes([accel_buf[0], accel_buf[1]]) as f32) / accel_scale * G;
    let accel_y = (i16::from_be_bytes([accel_buf[2], accel_buf[3]]) as f32) / accel_scale * G;
    let accel_z = (i16::from_be_bytes([accel_buf[4], accel_buf[5]]) as f32) / accel_scale * G;

    let accel_vector = RawAccelVector::new(accel_x, accel_y, accel_z);

    // Convert raw gyro data to deg/s
    let gyro_x = (i16::from_be_bytes([gyro_buf[0], gyro_buf[1]]) as f32) / gyro_scale;
    let gyro_y = (i16::from_be_bytes([gyro_buf[2], gyro_buf[3]]) as f32) / gyro_scale;
    let gyro_z = (i16::from_be_bytes([gyro_buf[4], gyro_buf[5]]) as f32) / gyro_scale;

    let gyro_vector = RawGyroVector::new(gyro_x, gyro_y, gyro_z);

    // Return IMU sample to task
    Ok(ImuSample::new(accel_vector, gyro_vector))
}
