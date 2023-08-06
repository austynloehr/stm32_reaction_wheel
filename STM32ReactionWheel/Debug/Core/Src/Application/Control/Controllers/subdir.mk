################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Control/Controllers/CT_Logging.c 

OBJS += \
./Core/Src/Application/Control/Controllers/CT_Logging.o 

C_DEPS += \
./Core/Src/Application/Control/Controllers/CT_Logging.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/Control/Controllers/%.o Core/Src/Application/Control/Controllers/%.su Core/Src/Application/Control/Controllers/%.cyclo: ../Core/Src/Application/Control/Controllers/%.c Core/Src/Application/Control/Controllers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Core/Inc/Application/Control/VirtualSensors -I../Core/Inc/Application/Control/Controllers -I../Core/Inc/Application -I../Core/Inc/Application/Config -I../Core/Inc/Application/Control -I../Core/Inc/Application/IO -I../Core/Inc/Application/Processing -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-Control-2f-Controllers

clean-Core-2f-Src-2f-Application-2f-Control-2f-Controllers:
	-$(RM) ./Core/Src/Application/Control/Controllers/CT_Logging.cyclo ./Core/Src/Application/Control/Controllers/CT_Logging.d ./Core/Src/Application/Control/Controllers/CT_Logging.o ./Core/Src/Application/Control/Controllers/CT_Logging.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-Control-2f-Controllers

