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
    pub motor_request_tx: SignalSender<MotorRequest>,
    pub motor_request_rx: SignalReceiver<MotorRequest>,
    pub vesc_status_tx: SignalSender<Frame>,
    pub vesc_status_rx: SignalReceiver<Frame>,
    pub green_led_tx: SignalSender<LedState>,
    pub green_led_rx: SignalReceiver<LedState>,
    pub red_led_tx: SignalSender<LedState>,
    pub red_led_rx: SignalReceiver<LedState>,
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

        // Create senders and receivers
        Some(Self {
            motor_request_tx: SignalSender::new(&MOTOR_REQUEST),
            motor_request_rx: SignalReceiver::new(&MOTOR_REQUEST),
            vesc_status_tx: SignalSender::new(&VESC_STATUS),
            vesc_status_rx: SignalReceiver::new(&VESC_STATUS),
            green_led_tx: SignalSender::new(&GREEN_LED),
            green_led_rx: SignalReceiver::new(&GREEN_LED),
            red_led_tx: SignalSender::new(&RED_LED),
            red_led_rx: SignalReceiver::new(&RED_LED),
        })
    }
}
