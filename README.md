# STM32 Reaction Wheel Development

This repo is used to store all development information for STM32 reaction wheel project.

## Components
- **MCU:** STM32F411CEU6 (Black Pill)
- **Motor:**  12V Pololu 37D, 10:1 gear ratio  
- **Motor Controller:** Roboclaw 2x7A
- **IMU:** MPU-6050
- **BLE:** HC-05

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