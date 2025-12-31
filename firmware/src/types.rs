use defmt::error;
use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_stm32::can::{CanRx, CanTx};
use embassy_stm32::i2c::{self, I2c};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::mutex::Mutex;

use common::types::AsyncCanHal;

// Re-export shared types from common crate
pub use common::types::{ImuSample, RawAccelVector, RawGyroVector, RxEvent, TxEvent};

/// Shared I2C Bus Type
pub type SharedI2c = I2cDevice<
    'static,
    CriticalSectionRawMutex,
    I2c<'static, embassy_stm32::mode::Async, i2c::Master>,
>;

/// Shared CAN Driver (Mutex-protected, Split Tx/Rx)
#[derive(Clone, Copy)]
pub struct SharedCan {
    pub tx: &'static Mutex<CriticalSectionRawMutex, CanTx<'static>>,
    pub rx: &'static Mutex<CriticalSectionRawMutex, CanRx<'static>>,
}

// Need to do this since there isnt a generic CAN HAL trait
impl AsyncCanHal for SharedCan {
    type Error = ();
    type Frame = embassy_stm32::can::Frame;

    async fn write(&mut self, frame: &Self::Frame) -> Result<(), Self::Error> {
        // Lock TX mutex only
        let mut tx = self.tx.lock().await;

        // Async write waits for a free mailbox, so it always succeeds.
        // Returns TransmitStatus with mailbox info, which we ignore.
        let _ = tx.write(frame).await;
        Ok(())
    }

    async fn read(&mut self) -> Result<Self::Frame, Self::Error> {
        // Lock RX mutex only
        let mut rx = self.rx.lock().await;

        rx.read().await.map(|envelope| envelope.frame).map_err(|e| {
            error!("CAN Rx Error: {:?}", e); // Just try to log error if read fails
        })
    }
}
