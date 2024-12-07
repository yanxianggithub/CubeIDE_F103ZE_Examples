################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/SYSTEM/sys/subdir.mk
-include Drivers/SYSTEM/delay/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/TOUCH/subdir.mk
-include Drivers/BSP/LED/subdir.mk
-include Drivers/BSP/LCD/subdir.mk
-include Drivers/BSP/KEY/subdir.mk
-include Drivers/BSP/IIC/subdir.mk
-include Drivers/BSP/BEEP/subdir.mk
-include Drivers/BSP/24CXX/subdir.mk
-include Core/Startup/subdir.mk
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


BUILD_ARTIFACT_NAME := 42_3_TOUCH_SRAM_APP
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
42_3_TOUCH_SRAM_APP.elf \

MAP_FILES += \
42_3_TOUCH_SRAM_APP.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
42_3_TOUCH_SRAM_APP.list \

OBJCOPY_BIN += \
42_3_TOUCH_SRAM_APP.bin \


# All Target
all: main-build

# Main-build Target
main-build: 42_3_TOUCH_SRAM_APP.elf secondary-outputs

# Tool invocations
42_3_TOUCH_SRAM_APP.elf 42_3_TOUCH_SRAM_APP.map: $(OBJS) $(USER_OBJS) H:\cubeide\STM32CubeIDE_DEMO\42_3_TOUCH_SRAM_APP\STM32F103ZETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "42_3_TOUCH_SRAM_APP.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"H:\cubeide\STM32CubeIDE_DEMO\42_3_TOUCH_SRAM_APP\STM32F103ZETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="42_3_TOUCH_SRAM_APP.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

42_3_TOUCH_SRAM_APP.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "42_3_TOUCH_SRAM_APP.list"
	@echo 'Finished building: $@'
	@echo ' '

42_3_TOUCH_SRAM_APP.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "42_3_TOUCH_SRAM_APP.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 42_3_TOUCH_SRAM_APP.bin 42_3_TOUCH_SRAM_APP.elf 42_3_TOUCH_SRAM_APP.list 42_3_TOUCH_SRAM_APP.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
