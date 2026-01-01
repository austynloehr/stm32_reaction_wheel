use common::types::AsyncCanHal;
use embedded_can::{ExtendedId, Frame, Id};

use common::types::MotorStatus;

#[derive(Debug, Clone, Copy)]
pub enum VescMode {
    Current,
    Speed,
}

/// VESC Driver Struct
pub struct Vesc<C> {
    can: C,
}

const MAX_CURRENT_MA: i32 = 20000;
const MAX_SPEED_RPM: i32 = 11000;
const NUM_POLES: i32 = 14;
const MAX_SPEED_ERPM: i32 = MAX_SPEED_RPM * (NUM_POLES / 2);

impl<C> Vesc<C>
where
    C: AsyncCanHal,
{
    /// Create a new VESC driver instance
    pub fn new(can: C) -> Self {
        Self { can }
    }

    /// Attempt to read a VESC status message, loops until a valid message is received
    pub async fn read_status(&mut self) -> Result<MotorStatus, C::Error> {
        loop {
            let frame = self.can.read().await?;

            let id = frame.id();
            let data = frame.data();

            if id == Id::Extended(ExtendedId::new(0x901).unwrap()) {
                if data.len() < 6 {
                    continue;
                }
                // Parse speed (big-endian)
                let speed_erpm = i32::from_be_bytes([data[0], data[1], data[2], data[3]]);

                // Parse current (big-endian)
                let current_da = i16::from_be_bytes([data[4], data[5]]) as i32;
                let current_ma = current_da * 100;

                // Convert erpm to rpm
                let speed_rpm = speed_erpm / (NUM_POLES / 2);

                return Ok(MotorStatus::new(speed_rpm, current_ma));
            }
        }
    }

    /// Send a VESC command
    pub async fn send_command(&mut self, mode: VescMode, request: i32) -> Result<(), C::Error> {
        let (can_id, command) = match mode {
            VescMode::Current => {
                let id = Id::Extended(ExtendedId::new(0x101).unwrap());
                let cmd = request.clamp(-MAX_CURRENT_MA, MAX_CURRENT_MA);
                (id, cmd)
            }
            VescMode::Speed => {
                let id = Id::Extended(ExtendedId::new(0x301).unwrap());
                let erpm = request * (NUM_POLES / 2);
                let cmd = erpm.clamp(-MAX_SPEED_ERPM, MAX_SPEED_ERPM);
                (id, cmd)
            }
        };

        // Convert command to bytes (big-endian)
        let payload = command.to_be_bytes();

        // Try to create a frame and send it
        if let Some(frame) = C::Frame::new(can_id, &payload) {
            self.can.write(&frame).await?;
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::mocks::can::{MockCan, TestFrame};
    use embedded_can::{ExtendedId, Id};
    use mockall::predicate::*;
    use rstest::*;

    /// Helper function to run async functions without runtime
    fn run_async<F, T>(f: F) -> T
    where
        F: std::future::Future<Output = T>,
    {
        futures::executor::block_on(f)
    }

    /// Helper function to create a test command frame
    fn create_test_frame(mode: VescMode, request: i32) -> TestFrame {
        match mode {
            VescMode::Current => {
                let id = Id::Extended(ExtendedId::new(0x101).unwrap());
                let cmd: i32 = request.clamp(-MAX_CURRENT_MA, MAX_CURRENT_MA);
                let payload: [u8; 4] = cmd.to_be_bytes();
                TestFrame::new(id, &payload).unwrap()
            }
            VescMode::Speed => {
                let id = Id::Extended(ExtendedId::new(0x301).unwrap());
                let erpm: i32 = request * (NUM_POLES / 2);
                let cmd: i32 = erpm.clamp(-MAX_SPEED_ERPM, MAX_SPEED_ERPM);
                let payload: [u8; 4] = cmd.to_be_bytes();
                TestFrame::new(id, &payload).unwrap()
            }
        }
    }

    /// Test that read_status properly parses a VESC status message
    #[rstest]
    fn test_read_status() {
        let mut mock = MockCan::new();

        // Create test data
        let speed_erpm: i32 = 14000;
        let current_da: i16 = 150;
        let mut payload: [u8; 6] = [0u8; 6];
        let speed_bytes: [u8; 4] = speed_erpm.to_be_bytes();
        let current_bytes: [u8; 2] = current_da.to_be_bytes();
        payload[0..4].copy_from_slice(&speed_bytes);
        payload[4..6].copy_from_slice(&current_bytes);

        let frame =
            TestFrame::new(Id::Extended(ExtendedId::new(0x901).unwrap()), &payload).unwrap();

        // Set read expectation and inject return frame
        mock.expect_read().times(1).return_const(Ok(frame));

        // Call read_status
        let mut vesc = Vesc::new(mock);
        let status = run_async(async { vesc.read_status().await });

        // Check result
        let expected_speed: i32 = speed_erpm / (NUM_POLES / 2);
        let expected_current: i32 = current_da as i32 * 100;
        assert_eq!(
            status,
            Ok(MotorStatus::new(expected_speed, expected_current))
        );
    }

    /// Test that send_command pack the request correctly and send to proper can ID
    #[rstest]
    #[case::current_mode(VescMode::Current, 1500)]
    #[case::speed_mode(VescMode::Speed, 2000)]
    fn test_send_command(#[case] mode: VescMode, #[case] request: i32) {
        let mut mock = MockCan::new();

        let expected_frame = create_test_frame(mode, request);

        // Set write expectation and inject return frame
        mock.expect_write()
            .with(eq(expected_frame))
            .times(1)
            .return_const(Ok(()));

        // Call send_command
        let mut vesc = Vesc::new(mock);
        let result = run_async(async { vesc.send_command(mode, request).await });

        // Check result
        assert_eq!(result, Ok(()));
    }

    /// Test that send_command clamps the request to the maximum values
    #[rstest]
    #[case::max_current(VescMode::Current, MAX_CURRENT_MA + 100, MAX_CURRENT_MA)]
    #[case::min_current(VescMode::Current, -MAX_CURRENT_MA - 100, -MAX_CURRENT_MA)]
    #[case::max_speed(VescMode::Speed, MAX_SPEED_ERPM + 100, MAX_SPEED_ERPM)]
    #[case::min_speed(VescMode::Speed, -MAX_SPEED_ERPM - 100, -MAX_SPEED_ERPM)]
    fn test_command_limits(#[case] mode: VescMode, #[case] request: i32, #[case] expected: i32) {
        let mut mock = MockCan::new();

        let expected_frame = create_test_frame(mode, expected);

        // Set write expectation and inject return frame
        mock.expect_write()
            .with(eq(expected_frame))
            .times(1)
            .return_const(Ok(()));

        // Call send_command
        let mut vesc = Vesc::new(mock);
        let result = run_async(async { vesc.send_command(mode, request).await });

        // Check result
        assert_eq!(result, Ok(()));
    }
}
