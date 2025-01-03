# STM32 Reaction Wheel Development

This repo is used to store all development information for STM32 reaction wheel project.

## Components
- **MCU:** STM32F412CGU6
- **Motor:**  REV Neo 550 (Brushless)
- **Motor Controller:** VESC EDU
- **CAN Transceiver:** NXP TJA1051/T3
- **IMU:** MPU-6050
- **BLE:** HC-05

## Project Structure

- Hardware: All hardware design files for this project
    - Balance_Controller: All KiCAD design files  for controller PCB
    - Reaction_Wheel: All NX CAD files for this project
- Logs: Bluetooth data logs
- ReactionWheel_F412: STM32CubeIDE project, contains all code for MCU
    - Core: Core source code
    - Drivers: STM32 HAL drivers
    - Middlewares: External libraries
- Resources: Markdowns and datasheets
- Scripts: Scripts useful for design and data logging