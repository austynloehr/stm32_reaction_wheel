################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.c \
../Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.c 

OBJS += \
./Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.o \
./Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.o 

C_DEPS += \
./Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.d \
./Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/Control/VirtualSensors/StateRequest/%.o Core/Src/Application/Control/VirtualSensors/StateRequest/%.su Core/Src/Application/Control/VirtualSensors/StateRequest/%.cyclo: ../Core/Src/Application/Control/VirtualSensors/StateRequest/%.c Core/Src/Application/Control/VirtualSensors/StateRequest/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Cx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Config" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/IO" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Processing" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Middlewares/ST/ARM/DSP/Inc" -I../Middlewares/ST/ARM/DSP/Inc -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/VirtualSensors/StateRequest" -I"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Core/Inc/Application/Control/Controllers/PrimaryStateMachine" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-Control-2f-VirtualSensors-2f-StateRequest

clean-Core-2f-Src-2f-Application-2f-Control-2f-VirtualSensors-2f-StateRequest:
	-$(RM) ./Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.cyclo ./Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.d ./Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.o ./Core/Src/Application/Control/VirtualSensors/StateRequest/StateRequest.su ./Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.cyclo ./Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.d ./Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.o ./Core/Src/Application/Control/VirtualSensors/StateRequest/VS_StateRequest.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-Control-2f-VirtualSensors-2f-StateRequest

