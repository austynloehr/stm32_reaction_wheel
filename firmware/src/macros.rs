// Firmware-specific macros

/// Monitors task loop frequency.
/// usage: monitor_task_rate!(unique_name, threshold_ms, tolerance_percent);
#[macro_export]
macro_rules! monitor_task_rate {
    ($name:ident, $threshold_ms:expr, $tolerance_pct:expr) => {
        paste::paste! {
            static [<$name:upper _LAST_RUN>]: embassy_sync::blocking_mutex::Mutex<
                embassy_sync::blocking_mutex::raw::CriticalSectionRawMutex,
                core::cell::Cell<Option<embassy_time::Instant>>
            > = embassy_sync::blocking_mutex::Mutex::new(
                core::cell::Cell::new(None)
            );

            const [<$name:upper _THRESHOLD>]: embassy_time::Duration =
                embassy_time::Duration::from_micros(
                    ($threshold_ms * 1000 * (100 + $tolerance_pct as u64) / 100) as u64
                );

            {
                let now = embassy_time::Instant::now();
                [<$name:upper _LAST_RUN>].lock(|cell| {
                    if let Some(last_instant) = cell.get() {
                        let elapsed = now.duration_since(last_instant);
                        if elapsed > [<$name:upper _THRESHOLD>] {
                            defmt::error!(
                                "{} task rate violation: {}us since last execution (threshold: {}us)",
                                stringify!($name),
                                elapsed.as_micros(),
                                [<$name:upper _THRESHOLD>].as_micros()
                            );
                        }
                    }
                    cell.set(Some(now));
                });
            }
        }
    };
}
