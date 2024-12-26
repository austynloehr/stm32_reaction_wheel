# STM32 Reaction Wheel Development

This repo is used to store all development information for STM32 reaction wheel project.

## Components
- **MCU:** STM32F412CGU6
- **Motor:**  REV Neo 550 (Brushless)
- **Motor Controller:** VESC EDU
- **CAN Transceiver:** NXP TJA1051/T3
- **IMU:** MPU-6050
- **BLE:** RN4871

## Software Architecture
This project has been split up into several source files in an effort to abstract the core control logic from the specific hardware. The overall software archecture is described in the block diagrams below.

### Application Block Diagram
![Application Block Diagram](Resources/BlockDiagrams/STM32_Reaction_Wheel_Architecture-Application.drawio.png)

### IO Block Diagram
![IO Block Diagram](Resources/BlockDiagrams/STM32_Reaction_Wheel_Architecture-IO.drawio.png)

### Processing Block Diagram
![Processing Block Diagram](Resources/BlockDiagrams/STM32_Reaction_Wheel_Architecture-Processing.drawio.png)

### Control Block Diagram
![Control Block Diagram](Resources/BlockDiagrams/STM32_Reaction_Wheel_Architecture-Control.drawio.png)