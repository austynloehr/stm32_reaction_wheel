use embedded_hal::digital::OutputPin;

/// A generic LED driver wrapper around an OutputPin.
///
/// Handles active-high vs active-low logic and tracks state locally
/// to allow toggling even for pins that are not readable.
pub struct Led<P> {
    pin: P,
    active_high: bool,
    state: bool,
}

impl<P: OutputPin> Led<P> {
    /// Create a new LED driver and set it to the initial state.
    ///
    /// # Arguments
    /// * `pin` - The GPIO pin implementation (must implement `OutputPin`)
    /// * `active_high` - `true` if setting the pin high turns the LED on
    /// * `initial_state` - The initial state of the LED (`true` for ON, `false` for OFF)
    ///
    /// # Errors
    /// Returns an error if setting the initial pin state fails
    pub fn new(mut pin: P, active_high: bool, initial_state: bool) -> Result<Self, P::Error> {
        // Apply initial state immediately
        if initial_state {
            if active_high {
                pin.set_high()?;
            } else {
                pin.set_low()?;
            }
        } else if active_high {
            pin.set_low()?;
        } else {
            pin.set_high()?;
        }

        Ok(Self {
            pin,
            active_high,
            state: initial_state,
        })
    }

    /// Turn the LED on
    pub fn on(&mut self) -> Result<(), P::Error> {
        if self.active_high {
            self.pin.set_high()?;
        } else {
            self.pin.set_low()?;
        }
        self.state = true;
        Ok(())
    }

    /// Turn the LED off
    pub fn off(&mut self) -> Result<(), P::Error> {
        if self.active_high {
            self.pin.set_low()?;
        } else {
            self.pin.set_high()?;
        }
        self.state = false;
        Ok(())
    }

    /// Toggle the LED state
    pub fn toggle(&mut self) -> Result<(), P::Error> {
        if self.state { self.off() } else { self.on() }
    }

    /// Check if the LED is currently considered on last command
    pub fn state(&self) -> bool {
        self.state
    }
}
