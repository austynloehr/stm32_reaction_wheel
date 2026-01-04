use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_stm32::i2c::{self, I2c};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::signal::Signal;

// Re-export shared types from common crate
pub use common::types::*;

/// Shared I2C Bus Type
pub type SharedI2c = I2cDevice<
    'static,
    CriticalSectionRawMutex,
    I2c<'static, embassy_stm32::mode::Async, i2c::Master>,
>;

/// Write-only access to a Signal
pub struct SignalSender<T: Send + 'static> {
    signal: &'static Signal<CriticalSectionRawMutex, T>,
}

impl<T: Send + 'static> SignalSender<T> {
    pub fn new(signal: &'static Signal<CriticalSectionRawMutex, T>) -> Self {
        Self { signal }
    }

    /// Send a value to the signal, overwriting any previous value.
    /// Notifies any waiting tasks.
    pub fn signal(&self, val: T) {
        self.signal.signal(val);
    }
}

/// Read-only access to a Signal
pub struct SignalReceiver<T: Send + 'static> {
    signal: &'static Signal<CriticalSectionRawMutex, T>,
}

impl<T: Send + 'static> SignalReceiver<T> {
    pub fn new(signal: &'static Signal<CriticalSectionRawMutex, T>) -> Self {
        Self { signal }
    }

    /// Wait for the signal to be signaled, then return the value.
    pub async fn wait(&self) -> T {
        self.signal.wait().await
    }
}
