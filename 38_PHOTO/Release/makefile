################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include Middlewares/Third_Party/FatFs/exfuns/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/SYSTEM/sys/subdir.mk
-include Drivers/SYSTEM/delay/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/OV7725/subdir.mk
-include Drivers/BSP/NORFLASH/subdir.mk
-include Drivers/BSP/LED/subdir.mk
-include Drivers/BSP/LCD/subdir.mk
-include Drivers/BSP/KEY/subdir.mk
-include Drivers/BSP/EXTI/subdir.mk
-include Drivers/BSP/BEEP/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include ATK_Middlewares/USMART/subdir.mk
-include ATK_Middlewares/TEXT/subdir.mk
-include ATK_Middlewares/PICTURE/subdir.mk
-include ATK_Middlewares/MALLOC/subdir.mk
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


BUILD_ARTIFACT_NAME := 38_PHOTO
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
38_PHOTO.elf \

MAP_FILES += \
38_PHOTO.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
38_PHOTO.list \


# All Target
all: main-build

# Main-build Target
main-build: 38_PHOTO.elf secondary-outputs

# Tool invocations
38_PHOTO.elf 38_PHOTO.map: $(OBJS) $(USER_OBJS) H:\cubeide\STM32CubeIDE_DEMO\38_PHOTO\STM32F103ZETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "38_PHOTO.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"H:\cubeide\STM32CubeIDE_DEMO\38_PHOTO\STM32F103ZETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="38_PHOTO.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

38_PHOTO.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "38_PHOTO.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 38_PHOTO.elf 38_PHOTO.list 38_PHOTO.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
