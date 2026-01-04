//! # System Channels
//!
//! This module holds a singleton of system channels that can be used to communicate between tasks.

use crate::types::{CanFrame, RxEvent, TxEvent};
use core::sync::atomic::{AtomicBool, Ordering};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Channel;
use static_cell::StaticCell;

static CHANNELS_TAKEN: AtomicBool = AtomicBool::new(false);

/// Holds references to all system channels.
#[allow(dead_code)]
#[derive(Clone, Copy)]
pub struct TaskChannels {
    pub input_channel: &'static Channel<CriticalSectionRawMutex, RxEvent, 128>,
    pub output_channel: &'static Channel<CriticalSectionRawMutex, TxEvent, 128>,
    pub can_tx_channel: &'static Channel<CriticalSectionRawMutex, CanFrame, 128>,
}

impl TaskChannels {
    /// Retrieves the system channels singleton.
    ///
    /// This function returns `Some(TaskChannels)` on the first call, and `None` on subsequent calls.
    pub fn take() -> Option<Self> {
        if CHANNELS_TAKEN.swap(true, Ordering::Relaxed) {
            return None;
        }

        // Initialize channels in static memory
        static INPUT_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, RxEvent, 128>> =
            StaticCell::new();
        let input_channel = INPUT_CHANNEL.init(Channel::new());

        static OUTPUT_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, TxEvent, 128>> =
            StaticCell::new();
        let output_channel = OUTPUT_CHANNEL.init(Channel::new());

        static CAN_TX_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, CanFrame, 128>> =
            StaticCell::new();
        let can_tx_channel = CAN_TX_CHANNEL.init(Channel::new());

        Some(Self {
            input_channel,
            output_channel,
            can_tx_channel,
        })
    }
}
