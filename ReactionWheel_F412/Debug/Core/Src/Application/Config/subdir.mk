################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Config/Config_DSP.c \
../Core/Src/Application/Config/Config_MPU6050.c \
../Core/Src/Application/Config/Config_VESC.c 

OBJS += \
./Core/Src/Application/Config/Config_DSP.o \
./Core/Src/Application/Config/Config_MPU6050.o \
./Core/Src/Application/Config/Config_VESC.o 

C_DEPS += \
./Core/Src/Application/Config/Config_DSP.d \
./Core/Src/Application/Config/Config_MPU6050.d \
./Core/Src/Application/Config/Config_VESC.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/Config/%.o Core/Src/Application/Config/%.su Core/Src/Application/Config/%.cyclo: ../Core/Src/Application/Config/%.c Core/Src/Application/Config/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Cx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Config" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/IO" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Processing" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Middlewares/ST/ARM/DSP/Inc" -I../Middlewares/ST/ARM/DSP/Inc -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors/StateRequest" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers/PrimaryStateMachine" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-Config

clean-Core-2f-Src-2f-Application-2f-Config:
	-$(RM) ./Core/Src/Application/Config/Config_DSP.cyclo ./Core/Src/Application/Config/Config_DSP.d ./Core/Src/Application/Config/Config_DSP.o ./Core/Src/Application/Config/Config_DSP.su ./Core/Src/Application/Config/Config_MPU6050.cyclo ./Core/Src/Application/Config/Config_MPU6050.d ./Core/Src/Application/Config/Config_MPU6050.o ./Core/Src/Application/Config/Config_MPU6050.su ./Core/Src/Application/Config/Config_VESC.cyclo ./Core/Src/Application/Config/Config_VESC.d ./Core/Src/Application/Config/Config_VESC.o ./Core/Src/Application/Config/Config_VESC.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-Config

