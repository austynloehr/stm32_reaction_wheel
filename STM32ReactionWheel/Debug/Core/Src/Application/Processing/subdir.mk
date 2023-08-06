################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/Processing/PR_HC05.c \
../Core/Src/Application/Processing/PR_MPU6050.c \
../Core/Src/Application/Processing/Processing.c 

OBJS += \
./Core/Src/Application/Processing/PR_HC05.o \
./Core/Src/Application/Processing/PR_MPU6050.o \
./Core/Src/Application/Processing/Processing.o 

C_DEPS += \
./Core/Src/Application/Processing/PR_HC05.d \
./Core/Src/Application/Processing/PR_MPU6050.d \
./Core/Src/Application/Processing/Processing.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/Processing/%.o Core/Src/Application/Processing/%.su Core/Src/Application/Processing/%.cyclo: ../Core/Src/Application/Processing/%.c Core/Src/Application/Processing/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Core/Inc/Application/Control/VirtualSensors -I../Core/Inc/Application/Control/Controllers -I../Core/Inc/Application -I../Core/Inc/Application/Config -I../Core/Inc/Application/Control -I../Core/Inc/Application/IO -I../Core/Inc/Application/Processing -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-Processing

clean-Core-2f-Src-2f-Application-2f-Processing:
	-$(RM) ./Core/Src/Application/Processing/PR_HC05.cyclo ./Core/Src/Application/Processing/PR_HC05.d ./Core/Src/Application/Processing/PR_HC05.o ./Core/Src/Application/Processing/PR_HC05.su ./Core/Src/Application/Processing/PR_MPU6050.cyclo ./Core/Src/Application/Processing/PR_MPU6050.d ./Core/Src/Application/Processing/PR_MPU6050.o ./Core/Src/Application/Processing/PR_MPU6050.su ./Core/Src/Application/Processing/Processing.cyclo ./Core/Src/Application/Processing/Processing.d ./Core/Src/Application/Processing/Processing.o ./Core/Src/Application/Processing/Processing.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-Processing

