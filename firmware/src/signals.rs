//! # System Signals
//!
//! This module provides a singleton of system signals that can be used to communicate between tasks.

use crate::types::{LedState, MotorRequest, SignalReceiver, SignalSender};
use core::sync::atomic::{AtomicBool, Ordering};
use embassy_stm32::can::Frame;
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::signal::Signal;

static SIGNALS_TAKEN: AtomicBool = AtomicBool::new(false);

pub struct TaskSignals {
    pub motor_request: SignalPair<MotorRequest>,
    pub vesc_status: SignalPair<Frame>,
    pub green_led: SignalPair<LedState>,
    pub red_led: SignalPair<LedState>,
}

// Wrap a signal into a pair with sender and receiver
pub struct SignalPair<T: Send + 'static> {
    pub sender: SignalSender<T>,
    pub receiver: SignalReceiver<T>,
}

impl<T: Send + 'static> SignalPair<T> {
    pub fn new(signal: &'static Signal<CriticalSectionRawMutex, T>) -> Self {
        Self {
            sender: SignalSender::new(signal),
            receiver: SignalReceiver::new(signal),
        }
    }
}

impl TaskSignals {
    /// Take TaskSignals singleton
    ///
    /// Returns Some(TaskSignals) on the first call, and None on subsequent calls.
    pub fn take() -> Option<Self> {
        if SIGNALS_TAKEN.swap(true, Ordering::Relaxed) {
            return None;
        }

        // Create base signals to share with tasks
        static MOTOR_REQUEST: Signal<CriticalSectionRawMutex, MotorRequest> = Signal::new();
        static VESC_STATUS: Signal<CriticalSectionRawMutex, Frame> = Signal::new();
        static GREEN_LED: Signal<CriticalSectionRawMutex, LedState> = Signal::new();
        static RED_LED: Signal<CriticalSectionRawMutex, LedState> = Signal::new();

        // Create signal pairs
        Some(Self {
            motor_request: SignalPair::new(&MOTOR_REQUEST),
            vesc_status: SignalPair::new(&VESC_STATUS),
            green_led: SignalPair::new(&GREEN_LED),
            red_led: SignalPair::new(&RED_LED),
        })
    }
}
