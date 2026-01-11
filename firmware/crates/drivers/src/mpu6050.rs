//! # MPU6050 Driver
//!
//! This module provides a driver for the MPU6050 accelerometer and gyroscope.

use common::types::*;
use core::marker::PhantomData;

/// MPU6050 driver errors
#[derive(Debug, Clone, Copy, defmt::Format)]
pub enum Mpu6050Error<I2cError> {
    WakeupFailed(I2cError),
    SampleRateConfigFailed(I2cError),
    DlpfConfigFailed(I2cError),
    AccelConfigFailed(I2cError),
    GyroConfigFailed(I2cError),
    AccelReadFailed(I2cError),
    GyroReadFailed(I2cError),
}

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

// Typestate States
pub struct Uninitialized;
pub struct Initialized;

/// MPU6050 Driver Struct
pub struct Mpu6050<I, State = Uninitialized> {
    i2c: I,
    _state: PhantomData<State>,
}

impl<I> Mpu6050<I, Uninitialized>
where
    I: embedded_hal_async::i2c::I2c,
{
    /// Create a new MPU6050 driver instance
    pub fn new(i2c: I) -> Self {
        Self {
            i2c,
            _state: PhantomData,
        }
    }

    /// # MPU6050 Initialization
    ///
    /// Wake up and configure MPU6050 for application.
    /// consumes the uninitialized driver and returns an initialized one.
    ///
    /// # Returns
    ///
    /// * `Result<Mpu6050<I, Initialized>, Mpu6050Error<I::Error>>`
    pub async fn init(mut self) -> Result<Mpu6050<I, Initialized>, Mpu6050Error<I::Error>> {
        // Wake up (Clear sleep bit)
        self.i2c
            .write(MPU6050_ADDR, &[PWR_MGMT1_REG, 0x00])
            .await
            .map_err(Mpu6050Error::WakeupFailed)?;

        // Config Sample Rate Divider
        self.i2c
            .write(MPU6050_ADDR, &[SMPRT_DIV_REG, SMPLRT_DIV])
            .await
            .map_err(Mpu6050Error::SampleRateConfigFailed)?;

        // Config DLPF
        self.i2c
            .write(MPU6050_ADDR, &[DLPF_REG, DLPF_CFG])
            .await
            .map_err(Mpu6050Error::DlpfConfigFailed)?;

        // Config Accelerometer Range
        self.i2c
            .write(MPU6050_ADDR, &[ACCEL_CONFIG_REG, (AFS_SEL as u8) << 3])
            .await
            .map_err(Mpu6050Error::AccelConfigFailed)?;

        // Config Gyroscope Range
        self.i2c
            .write(MPU6050_ADDR, &[GYRO_CONFIG_REG, (FS_SEL as u8) << 3])
            .await
            .map_err(Mpu6050Error::GyroConfigFailed)?;

        Ok(Mpu6050 {
            i2c: self.i2c,
            _state: PhantomData,
        })
    }
}

impl<I> Mpu6050<I, Initialized>
where
    I: embedded_hal_async::i2c::I2c,
{
    /// # MPU6050 Read
    ///
    /// Read IMU data from MPU6050.
    ///
    /// # Returns
    ///
    /// * Result containing the IMU sample (accel and gyro data) or an error if the read fails.
    pub async fn read(&mut self) -> Result<ImuSample, Mpu6050Error<I::Error>> {
        // Buffer for accel and gyro data
        let mut accel_buf = [0u8; 6];
        let mut gyro_buf = [0u8; 6];

        // Read accel and gyro data
        // Return error if read fails
        self.i2c
            .write_read(MPU6050_ADDR, &[ACCEL_OUT_START_REG], &mut accel_buf)
            .await
            .map_err(Mpu6050Error::AccelReadFailed)?;
        self.i2c
            .write_read(MPU6050_ADDR, &[GYRO_OUT_START_REG], &mut gyro_buf)
            .await
            .map_err(Mpu6050Error::GyroReadFailed)?;

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

    fn configure_init_expectations(mock: &mut MockI2c) {
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
    }

    #[rstest]
    fn test_init() {
        let mut mock = MockI2c::new();
        configure_init_expectations(&mut mock);

        run_async(async {
            let imu = Mpu6050::new(mock);
            imu.init().await.unwrap();
        });
    }

    #[rstest]
    #[case::zeros(
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        (0.0, 0.0, 0.0),
        (0.0, 0.0, 0.0)
    )]
    #[case::one_g_z([0, 0, 0, 0, 0x10, 0x00], [0; 6], (0.0, 0.0, 9.81), (0.0, 0.0, 0.0))]
    #[case::neg_one_g_z([0, 0, 0, 0, 0xF0, 0x00], [0; 6], (0.0, 0.0, -9.81), (0.0, 0.0, 0.0))]
    #[case::one_g_y([0, 0, 0x10, 0x00, 0, 0], [0; 6], (0.0, 9.81, 0.0), (0.0, 0.0, 0.0))]
    #[case::neg_one_g_y([0, 0, 0xF0, 0x00, 0, 0], [0; 6], (0.0, -9.81, 0.0), (0.0, 0.0, 0.0))]
    #[case::one_g_x([0x10, 0x00, 0, 0, 0, 0], [0; 6], (9.81, 0.0, 0.0), (0.0, 0.0, 0.0))]
    #[case::neg_one_g_x([0xF0, 0x00, 0, 0, 0, 0], [0; 6], (-9.81, 0.0, 0.0), (0.0, 0.0, 0.0))]
    #[case::max_positive(
        [0x7F, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF],
        [0x7F, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF],
        (78.48, 78.48, 78.48),
        (1997.98, 1997.98, 1997.98)
    )]
    #[case::max_negative(
        [0x80, 0x00, 0x80, 0x00, 0x80, 0x00],
        [0x80, 0x00, 0x80, 0x00, 0x80, 0x00],
        (-78.48, -78.48, -78.48),
        (-1998.05, -1998.05, -1998.05)
    )]
    fn test_read(
        #[case] accel_bytes: [u8; 6],
        #[case] gyro_bytes: [u8; 6],
        #[case] expected_accel: (f32, f32, f32),
        #[case] expected_gyro: (f32, f32, f32),
    ) {
        let mut mock = MockI2c::new();
        configure_init_expectations(&mut mock);

        // 6. Read Accel
        // We must clone the bytes because the closure needs to own them for valid lifetime in 'returning'
        let accel_bytes_clone = accel_bytes;
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 2
                    && matches!(&ops[0], Operation::Write(data) if data == &[ACCEL_OUT_START_REG])
                    && matches!(&ops[1], Operation::Read(buf) if buf.len() == 6)
            })
            .times(1)
            .returning(move |_, ops| {
                if let Operation::Read(buf) = &mut ops[1] {
                    buf.copy_from_slice(&accel_bytes_clone);
                }
                Ok(())
            });

        // 7. Read Gyro
        let gyro_bytes_clone = gyro_bytes;
        mock.expect_transaction()
            .withf(|addr, ops| {
                *addr == MPU6050_ADDR
                    && ops.len() == 2
                    && matches!(&ops[0], Operation::Write(data) if data == &[GYRO_OUT_START_REG])
                    && matches!(&ops[1], Operation::Read(buf) if buf.len() == 6)
            })
            .times(1)
            .returning(move |_, ops| {
                if let Operation::Read(buf) = &mut ops[1] {
                    buf.copy_from_slice(&gyro_bytes_clone);
                }
                Ok(())
            });

        // Run Test
        let sample = run_async(async {
            let imu = Mpu6050::new(mock);
            let mut imu = imu.init().await.unwrap();
            imu.read().await.unwrap()
        });

        assert_relative_eq!(sample.accel().data().x, expected_accel.0, epsilon = 0.1);
        assert_relative_eq!(sample.accel().data().y, expected_accel.1, epsilon = 0.1);
        assert_relative_eq!(sample.accel().data().z, expected_accel.2, epsilon = 0.1);

        assert_relative_eq!(sample.gyro().data().x, expected_gyro.0, epsilon = 0.1);
        assert_relative_eq!(sample.gyro().data().y, expected_gyro.1, epsilon = 0.1);
        assert_relative_eq!(sample.gyro().data().z, expected_gyro.2, epsilon = 0.1);
    }
}
