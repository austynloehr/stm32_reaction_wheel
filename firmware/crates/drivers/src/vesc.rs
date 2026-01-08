use common::types::{MotorCtrlMode, MotorStatus};
#[cfg(not(test))]
use defmt::*;

#[cfg(test)]
use log::error;

#[derive(Debug, Copy, Clone)]
pub struct Vesc {}

#[derive(Debug, Copy, Clone, PartialEq, defmt::Format)]
pub enum VescError {
    InvalidDataLength,
}

impl Default for Vesc {
    fn default() -> Self {
        Self::new()
    }
}

impl Vesc {
    pub const MAX_CURRENT_MA: i32 = 20000;
    pub const MAX_SPEED_RPM: i32 = 11000;
    pub const NUM_POLES: i32 = 14;
    pub const MAX_SPEED_ERPM: i32 = (Self::MAX_SPEED_RPM) * (Self::NUM_POLES / 2);
    pub const SPEED_CMD_ID: u32 = 0x301;
    pub const CURRENT_CMD_ID: u32 = 0x101;

    pub fn new() -> Self {
        Self {}
    }

    /// Unpack a VESC status message from raw CAN data
    pub fn unpack_status(&self, data: &[u8]) -> Result<MotorStatus, VescError> {
        if data.len() < 6 {
            error!("Attemped to unpack VESC status message with invalid data length");
            return Err(VescError::InvalidDataLength);
        }

        // Parse speed (big-endian)
        let speed_erpm = i32::from_be_bytes([data[0], data[1], data[2], data[3]]);
        let speed_rpm = speed_erpm / (Self::NUM_POLES / 2);

        // Parse current (big-endian)
        let current_da = i16::from_be_bytes([data[4], data[5]]) as i32;
        let current_ma = current_da * 100;

        Ok(MotorStatus::new(speed_rpm, current_ma))
    }

    /// Create command data for a VESC command
    /// Returns (CAN ID, data bytes, data length)
    pub fn create_command(&self, mode: MotorCtrlMode, request: i32) -> (u32, [u8; 8], usize) {
        // Get the ID and limit command based on the mode
        let (id, command) = match mode {
            MotorCtrlMode::Current => {
                let id = Self::CURRENT_CMD_ID;
                let cmd = request.clamp(-Self::MAX_CURRENT_MA, Self::MAX_CURRENT_MA);
                (id, cmd)
            }
            MotorCtrlMode::Speed => {
                let id = Self::SPEED_CMD_ID;
                let erpm = request * (Self::NUM_POLES / 2);
                let cmd = erpm.clamp(-Self::MAX_SPEED_ERPM, Self::MAX_SPEED_ERPM);
                (id, cmd)
            }
        };

        // Convert command to bytes (big-endian)
        let mut data = [0u8; 8];
        data[..4].copy_from_slice(&command.to_be_bytes());

        (id, data, 4)
    }

    /// Create fail-safe command data
    /// Returns (CAN ID, data bytes, data length)
    pub fn create_fail_safe(&self) -> (u32, [u8; 8], usize) {
        (Self::CURRENT_CMD_ID, [0u8; 8], 4)
    }
}

#[cfg(test)]
mod tests {
    use super::{MotorCtrlMode, MotorStatus, Vesc, VescError};
    use rstest::*;

    #[rstest]
    #[case::zeros(0, 0)]
    #[case::positive(100, 100)]
    #[case::negative(-100, -100)]
    #[case::max_values(Vesc::MAX_SPEED_RPM, Vesc::MAX_CURRENT_MA)]
    #[case::min_values(-Vesc::MAX_SPEED_RPM, -Vesc::MAX_CURRENT_MA)]
    fn test_unpack_status(#[case] speed_rpm: i32, #[case] current_ma: i32) {
        let vesc = Vesc::new();

        // Construct frame with test data
        let mut data = [0u8; 8];
        let speed_erpm = speed_rpm * (Vesc::NUM_POLES / 2);
        let current_da = (current_ma / 100) as i16;

        data[..4].copy_from_slice(&speed_erpm.to_be_bytes());
        data[4..6].copy_from_slice(&current_da.to_be_bytes());

        // Unpack frame
        let status = vesc.unpack_status(&data);

        // Assert unpacked values match expected test data
        assert_eq!(status, Ok(MotorStatus::new(speed_rpm, current_ma)));
    }

    #[rstest]
    fn test_unpack_status_invalid_len() {
        let vesc = Vesc::new();
        // Create frame with only 5 bytes (less than 6)
        let data = [0u8; 5];
        let status = vesc.unpack_status(&data);
        assert_eq!(status, Err(VescError::InvalidDataLength));
    }

    #[rstest]
    #[case::current(MotorCtrlMode::Current, 100, Vesc::CURRENT_CMD_ID)]
    #[case::speed(MotorCtrlMode::Speed, 1000, Vesc::SPEED_CMD_ID)]
    fn test_create_command(
        #[case] mode: MotorCtrlMode,
        #[case] val: i32,
        #[case] expected_id: u32,
    ) {
        let vesc = Vesc::new();
        let (id, data, len) = vesc.create_command(mode, val);

        assert_eq!(id, expected_id);
        assert_eq!(len, 4);

        let expected_cmd = match mode {
            MotorCtrlMode::Current => val,
            MotorCtrlMode::Speed => val * (Vesc::NUM_POLES / 2),
        };

        let expected_bytes = expected_cmd.to_be_bytes();
        assert_eq!(&data[..len], &expected_bytes);
    }

    #[rstest]
    fn test_create_fail_safe() {
        let vesc = Vesc::new();
        let (id, data, len) = vesc.create_fail_safe();

        assert_eq!(id, 0x101);
        assert_eq!(len, 4);
        assert_eq!(&data[..len], &[0u8, 0u8, 0u8, 0u8]);
    }

    #[rstest]
    #[case::max_current(MotorCtrlMode::Current, Vesc::MAX_CURRENT_MA + 100, Vesc::MAX_CURRENT_MA)]
    #[case::min_current(MotorCtrlMode::Current, -Vesc::MAX_CURRENT_MA - 100, -Vesc::MAX_CURRENT_MA)]
    #[case::max_speed(MotorCtrlMode::Speed, Vesc::MAX_SPEED_RPM + 100, Vesc::MAX_SPEED_RPM)]
    #[case::min_speed(MotorCtrlMode::Speed, -(Vesc::MAX_SPEED_RPM + 100), -Vesc::MAX_SPEED_RPM)]
    fn test_command_limits(
        #[case] mode: MotorCtrlMode,
        #[case] request: i32,
        #[case] expected_val: i32,
    ) {
        let vesc = Vesc::new();
        let (id, data, len) = vesc.create_command(mode, request);

        let (expected_cmd, expected_id) = match mode {
            MotorCtrlMode::Current => (expected_val, Vesc::CURRENT_CMD_ID),
            MotorCtrlMode::Speed => (expected_val * (Vesc::NUM_POLES / 2), Vesc::SPEED_CMD_ID),
        };

        let expected_bytes = expected_cmd.to_be_bytes();
        assert_eq!(&data[..len], &expected_bytes);
        assert_eq!(id, expected_id);
    }
}
