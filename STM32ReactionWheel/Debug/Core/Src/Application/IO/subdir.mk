################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/IO/IO.c \
../Core/Src/Application/IO/IO_HC05.c \
../Core/Src/Application/IO/IO_MPU6050.c 

OBJS += \
./Core/Src/Application/IO/IO.o \
./Core/Src/Application/IO/IO_HC05.o \
./Core/Src/Application/IO/IO_MPU6050.o 

C_DEPS += \
./Core/Src/Application/IO/IO.d \
./Core/Src/Application/IO/IO_HC05.d \
./Core/Src/Application/IO/IO_MPU6050.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/IO/%.o Core/Src/Application/IO/%.su Core/Src/Application/IO/%.cyclo: ../Core/Src/Application/IO/%.c Core/Src/Application/IO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Core/Inc/Application/Control/Controllers -I../Core/Inc/Application -I../Core/Inc/Application/Config -I../Core/Inc/Application/Control -I../Core/Inc/Application/IO -I../Core/Inc/Application/Processing -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-IO

clean-Core-2f-Src-2f-Application-2f-IO:
	-$(RM) ./Core/Src/Application/IO/IO.cyclo ./Core/Src/Application/IO/IO.d ./Core/Src/Application/IO/IO.o ./Core/Src/Application/IO/IO.su ./Core/Src/Application/IO/IO_HC05.cyclo ./Core/Src/Application/IO/IO_HC05.d ./Core/Src/Application/IO/IO_HC05.o ./Core/Src/Application/IO/IO_HC05.su ./Core/Src/Application/IO/IO_MPU6050.cyclo ./Core/Src/Application/IO/IO_MPU6050.d ./Core/Src/Application/IO/IO_MPU6050.o ./Core/Src/Application/IO/IO_MPU6050.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-IO

