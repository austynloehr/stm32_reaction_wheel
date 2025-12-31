#![cfg_attr(not(test), no_std)]

pub mod mpu6050;

#[cfg(test)]
pub mod mocks;
