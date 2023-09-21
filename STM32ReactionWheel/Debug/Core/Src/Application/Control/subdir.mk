################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Control/Control.c 

OBJS += \
./Core/Src/Application/Control/Control.o 

C_DEPS += \
./Core/Src/Application/Control/Control.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/Control/%.o Core/Src/Application/Control/%.su Core/Src/Application/Control/%.cyclo: ../Core/Src/Application/Control/%.c Core/Src/Application/Control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application/Config" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application/Control" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application/Control/Controllers" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application/Control/VirtualSensors" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application/IO" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Core/Inc/Application/Processing" -I"/home/aloehr/Repos/stm32_reaction_wheel/STM32ReactionWheel/Middlewares/ST/ARM/DSP/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-Control

clean-Core-2f-Src-2f-Application-2f-Control:
	-$(RM) ./Core/Src/Application/Control/Control.cyclo ./Core/Src/Application/Control/Control.d ./Core/Src/Application/Control/Control.o ./Core/Src/Application/Control/Control.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-Control

