################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f407vgtx.s 

OBJS += \
./Core/Startup/startup_stm32f407vgtx.o 

S_DEPS += \
./Core/Startup/startup_stm32f407vgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application/Config" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application/Control" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application/Control/Controllers" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application/Control/VirtualSensors" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application/IO" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F407/Core/Inc/Application/Processing" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f407vgtx.d ./Core/Startup/startup_stm32f407vgtx.o

.PHONY: clean-Core-2f-Startup

