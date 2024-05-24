################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.c \
../Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.c 

OBJS += \
./Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.o \
./Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.o 

C_DEPS += \
./Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.d \
./Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/Control/Controllers/PrimaryStateMachine/%.o Core/Src/Application/Control/Controllers/PrimaryStateMachine/%.su Core/Src/Application/Control/Controllers/PrimaryStateMachine/%.cyclo: ../Core/Src/Application/Control/Controllers/PrimaryStateMachine/%.c Core/Src/Application/Control/Controllers/PrimaryStateMachine/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Cx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Config" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/IO" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Processing" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Middlewares/ST/ARM/DSP/Inc" -I../Middlewares/ST/ARM/DSP/Inc -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors/StateRequest" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers/PrimaryStateMachine" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-Control-2f-Controllers-2f-PrimaryStateMachine

clean-Core-2f-Src-2f-Application-2f-Control-2f-Controllers-2f-PrimaryStateMachine:
	-$(RM) ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.cyclo ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.d ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.o ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/CT_PrimaryStateMachine.su ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.cyclo ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.d ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.o ./Core/Src/Application/Control/Controllers/PrimaryStateMachine/PrimaryStateMachine.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-Control-2f-Controllers-2f-PrimaryStateMachine

