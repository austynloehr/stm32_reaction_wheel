//! # System Channels
//!
//! This module holds a singleton of system channels that can be used to communicate between tasks.

use crate::types::{InputEvent, OutputEvent};
use core::sync::atomic::{AtomicBool, Ordering};
use embassy_stm32::can::Frame;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::channel::Channel;
use static_cell::StaticCell;

static CHANNELS_TAKEN: AtomicBool = AtomicBool::new(false);

/// Holds references to all system channels.
#[allow(dead_code)]
#[derive(Clone, Copy)]
pub struct TaskChannels {
    pub inputs: &'static Channel<CriticalSectionRawMutex, InputEvent, 128>,
    pub outputs: &'static Channel<CriticalSectionRawMutex, OutputEvent, 128>,
    pub can_tx: &'static Channel<CriticalSectionRawMutex, Frame, 128>,
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
        static INPUT_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, InputEvent, 128>> =
            StaticCell::new();
        let input_channel = INPUT_CHANNEL.init(Channel::new());

        static OUTPUT_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, OutputEvent, 128>> =
            StaticCell::new();
        let output_channel = OUTPUT_CHANNEL.init(Channel::new());

        static CAN_TX_CHANNEL: StaticCell<Channel<CriticalSectionRawMutex, Frame, 128>> =
            StaticCell::new();
        let can_tx_channel = CAN_TX_CHANNEL.init(Channel::new());

        Some(Self {
            inputs: input_channel,
            outputs: output_channel,
            can_tx: can_tx_channel,
        })
    }
}
