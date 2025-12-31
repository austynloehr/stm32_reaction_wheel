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
