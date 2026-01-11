use embedded_hal_async::i2c::{ErrorType, I2c, Operation};
use mockall::mock;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MockError {
    ArbitrationLoss,
    Bus,
    Nack,
    Other,
}

// Mock I2C Errors
impl embedded_hal_async::i2c::Error for MockError {
    fn kind(&self) -> embedded_hal_async::i2c::ErrorKind {
        use embedded_hal_async::i2c::ErrorKind;
        match self {
            MockError::ArbitrationLoss => ErrorKind::ArbitrationLoss,
            MockError::Bus => ErrorKind::Bus,
            MockError::Nack => {
                ErrorKind::NoAcknowledge(embedded_hal_async::i2c::NoAcknowledgeSource::Address)
            }
            MockError::Other => ErrorKind::Other,
        }
    }
}

// Mock I2C implementation
mock! {
    pub I2c {}

    impl ErrorType for I2c {
        type Error = MockError;
    }

    // All I2C operations are mocked with this single function
    // Allows mockall to check address and operation inputs of caller
    impl I2c for I2c {
        async fn transaction<'a>(
            &mut self,
            address: u8,
            operations: &mut [Operation<'a>],
        ) -> Result<(), MockError>;
    }
}
