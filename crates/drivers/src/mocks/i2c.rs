use embedded_hal_async::i2c::{ErrorType, I2c, Operation};
use mockall::mock;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MockError {
    ArbitrationLoss,
    Bus,
    Nack,
    Other,
}

impl embedded_hal_async::i2c::Error for MockError {
    fn kind(&self) -> embedded_hal_async::i2c::ErrorKind {
        use embedded_hal_async::i2c::ErrorKind;
        match self {
            MockError::ArbitrationLoss => ErrorKind::ArbitrationLoss,
            MockError::Bus => ErrorKind::Bus,
            MockError::Nack => {
                ErrorKind::NoAcknowledge(embedded_hal_async::i2c::NoAcknowledgeSource::Address)
            } // Simplified
            MockError::Other => ErrorKind::Other,
        }
    }
}

mock! {
    pub I2c {}

    impl ErrorType for I2c {
        type Error = MockError;
    }

    impl I2c for I2c {
        async fn transaction(
            &mut self,
            address: u8,
            operations: &mut [Operation<'_>],
        ) -> Result<(), MockError>;
    }
}
