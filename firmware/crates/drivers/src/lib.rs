#![cfg_attr(not(test), no_std)]
pub mod mpu6050;
pub mod vesc;

#[cfg(test)]
pub mod mocks;
