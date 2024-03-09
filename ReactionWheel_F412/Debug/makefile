################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/Application/Processing/subdir.mk
-include Core/Src/Application/IO/subdir.mk
-include Core/Src/Application/Control/VirtualSensors/subdir.mk
-include Core/Src/Application/Control/Controllers/subdir.mk
-include Core/Src/Application/Control/subdir.mk
-include Core/Src/Application/Config/subdir.mk
-include Core/Src/Application/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := ReactionWheel_F412
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ReactionWheel_F412.elf \

MAP_FILES += \
ReactionWheel_F412.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ReactionWheel_F412.list \


# All Target
all: main-build

# Main-build Target
main-build: ReactionWheel_F412.elf secondary-outputs

# Tool invocations
ReactionWheel_F412.elf ReactionWheel_F412.map: $(OBJS) $(USER_OBJS) /home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/STM32F412CGUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "ReactionWheel_F412.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/STM32F412CGUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ReactionWheel_F412.map" -Wl,--gc-sections -static -L"/home/aloehr/Repos/stm32_reaction_wheel/ReactionWheel_F412/Middlewares/ST/ARM/DSP/Lib" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ReactionWheel_F412.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ReactionWheel_F412.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ReactionWheel_F412.elf ReactionWheel_F412.list ReactionWheel_F412.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
