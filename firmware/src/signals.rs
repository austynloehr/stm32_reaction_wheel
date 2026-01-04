//! # System Signals
//!
//! This module provides a singleton of system signals that can be used to communicate between tasks.

use crate::types::{CanFrame, MotorRequest, SignalReceiver, SignalSender};
use core::sync::atomic::{AtomicBool, Ordering};
use embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex;
use embassy_sync::signal::Signal;

static SIGNALS_TAKEN: AtomicBool = AtomicBool::new(false);

pub struct TaskSignals {
    pub motor_request_tx: SignalSender<MotorRequest>,
    pub motor_request_rx: SignalReceiver<MotorRequest>,
    pub vesc_status_tx: SignalSender<CanFrame>,
    pub vesc_status_rx: SignalReceiver<CanFrame>,
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
        static VESC_STATUS: Signal<CriticalSectionRawMutex, CanFrame> = Signal::new();

        // Create senders and receivers
        Some(Self {
            motor_request_tx: SignalSender::new(&MOTOR_REQUEST),
            motor_request_rx: SignalReceiver::new(&MOTOR_REQUEST),
            vesc_status_tx: SignalSender::new(&VESC_STATUS),
            vesc_status_rx: SignalReceiver::new(&VESC_STATUS),
        })
    }
}
