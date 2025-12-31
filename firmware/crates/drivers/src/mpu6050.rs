//! # MPU6050 Driver
//!
//! This module provides a driver for the MPU6050 accelerometer and gyroscope.

use common::types::*;

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
    pub fn sensitivity(&self) -> f32 {
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
    pub fn sensitivity(&self) -> f32 {
        match self {
            Self::W250 => 131.0,
            Self::W500 => 65.5,
            Self::W1000 => 32.8,
            Self::W2000 => 16.4,
        }
    }
}

// Constant registers
const MPU6050_ADDR: u8 = 0x68;
const ACCEL_CONFIG_REG: u8 = 0x1C;
const GYRO_CONFIG_REG: u8 = 0x1B;
const PWR_MGMT1_REG: u8 = 0x6B;
const SMPRT_DIV_REG: u8 = 0x19;
const DLPF_REG: u8 = 0x1A;
const ACCEL_OUT_START_REG: u8 = 0x3B;
const GYRO_OUT_START_REG: u8 = 0x43;

// Constant config values
const AFS_SEL: AfsSel = AfsSel::G8;
const FS_SEL: FsSel = FsSel::W2000;
const SMPLRT_DIV: u8 = 0x07;
const DLPF_CFG: u8 = 0x00;

// Gravity
const G: f32 = 9.81;

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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::mocks::i2c::MockI2c;
    use embedded_hal_async::i2c::Operation;
    // use mockall::predicate::*;
    use approx::assert_relative_eq;
    use rstest::*;

    fn run_async<F, T>(f: F) -> T
    where
        F: std::future::Future<Output = T>,
    {
        futures::executor::block_on(f)
    }

    #[rstest]
    fn test_init_success() {
        let mut mock = MockI2c::new();

        // 1. Wake up (PWR_MGMT1)
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 1
                    && matches!(&ops[0], Operation::Write(data) if data == &[PWR_MGMT1_REG, 0x00])
            })
            .times(1)
            .returning(|_, _| Ok(()));

        // 2. Sample Rate (SMPRT_DIV)
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 1
                    && matches!(&ops[0], Operation::Write(data) if data == &[SMPRT_DIV_REG, SMPLRT_DIV])
            })
            .times(1)
            .returning(|_, _| Ok(()));

        // 3. DLPF
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 1
                    && matches!(&ops[0], Operation::Write(data) if data == &[DLPF_REG, DLPF_CFG])
            })
            .times(1)
            .returning(|_, _| Ok(()));

        // 4. Accel Range
        let expected_accel_val = (AFS_SEL as u8) << 3;
        mock.expect_transaction()
            .withf(move |addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 1
                    && matches!(&ops[0], Operation::Write(data) if data == &[ACCEL_CONFIG_REG, expected_accel_val])
            })
            .times(1)
            .returning(|_, _| Ok(()));

        // 5. Gyro Range
        let expected_gyro_val = (FS_SEL as u8) << 3;
        mock.expect_transaction()
            .withf(move |addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 1
                    && matches!(&ops[0], Operation::Write(data) if data == &[GYRO_CONFIG_REG, expected_gyro_val])
            })
            .times(1)
            .returning(|_, _| Ok(()));

        run_async(async {
            init(&mut mock).await.unwrap();
        });
    }

    #[rstest]
    fn test_step_success() {
        let mut mock = MockI2c::new();

        // 1. Expect Read of Accelerometer Data
        // We expect a Write (set register) then a Read (get data)
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 2
                    // First op: Write register address 0x3B
                    && matches!(&ops[0], Operation::Write(data) if data == &[ACCEL_OUT_START_REG])
                    // Second op: Read 6 bytes
                    && matches!(&ops[1], Operation::Read(buf) if buf.len() == 6)
            })
            .times(1)
            .returning(|_, ops| {
                // Inject data into read operation
                if let Operation::Read(buf) = &mut ops[1] {
                    // Simulate 1G on Z-axis.
                    // Sensitivity G8 = 4096 LSB/g.
                    // 1G = 4096 = 0x1000. Big Endian: [0x10, 0x00].
                    // Data: X_H, X_L, Y_H, Y_L, Z_H, Z_L
                    let data = [0, 0, 0, 0, 0x10, 0x00];
                    buf.copy_from_slice(&data);
                }
                Ok(())
            });

        // 2. Expect Read of Gyroscope Data
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 2
                    && matches!(&ops[0], Operation::Write(data) if data == &[GYRO_OUT_START_REG])
                    && matches!(&ops[1], Operation::Read(buf) if buf.len() == 6)
            })
            .times(1)
            .returning(|_, ops| {
                if let Operation::Read(buf) = &mut ops[1] {
                    // Simulate 0 deg/s on all axes
                    let data = [0; 6];
                    buf.copy_from_slice(&data);
                }
                Ok(())
            });

        let sample = run_async(async { step(&mut mock).await.unwrap() });

        assert_relative_eq!(sample.accel().data().z, 9.81, epsilon = 0.1);
        assert_relative_eq!(sample.gyro().data().x, 0.0);
    }
}
