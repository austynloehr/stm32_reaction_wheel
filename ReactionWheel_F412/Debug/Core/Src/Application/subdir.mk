################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Application.c 

OBJS += \
./Core/Src/Application/Application.o 

C_DEPS += \
./Core/Src/Application/Application.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/%.o Core/Src/Application/%.su Core/Src/Application/%.cyclo: ../Core/Src/Application/%.c Core/Src/Application/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Cx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Config" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/IO" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Processing" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Middlewares/ST/ARM/DSP/Inc" -I../Middlewares/ST/ARM/DSP/Inc -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors/StateRequest" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers/PrimaryStateMachine" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application

clean-Core-2f-Src-2f-Application:
	-$(RM) ./Core/Src/Application/Application.cyclo ./Core/Src/Application/Application.d ./Core/Src/Application/Application.o ./Core/Src/Application/Application.su

.PHONY: clean-Core-2f-Src-2f-Application

