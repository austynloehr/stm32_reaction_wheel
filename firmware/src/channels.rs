//! # System Channels
//!
//! This module initializes and holds all the inter-task communication channels.
//!
//! # Usage
//!
//! ```
//! let channels = channels::init();
//! spawner.spawn(some_task(channels.input_channel.sender())).unwrap();
//! ```

use crate::types::{RxEvent, TxEvent};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Channel;
use static_cell::StaticCell;

/// Holds references to all system channels.
#[allow(dead_code)]
pub struct TaskChannels {
    pub input_channel: &'static Channel<CriticalSectionRawMutex, RxEvent, 128>,
    pub output_channel: &'static Channel<CriticalSectionRawMutex, TxEvent, 128>,
}

/// # Initialize Channels
///
/// Creates and initializes all system channels.
///
/// # Returns
///
/// * `TaskChannels` - Struct containing references to all initialized channels.
pub fn init() -> TaskChannels {
    // Initialize channels in static memory
    static INPUT_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, RxEvent, 128>> =
        StaticCell::new();
    let input_channel = INPUT_CHANNEL.init(Channel::new());

    static OUTPUT_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, TxEvent, 128>> =
        StaticCell::new();
    let output_channel = OUTPUT_CHANNEL.init(Channel::new());

    TaskChannels {
        input_channel,
        output_channel,
    }
}
