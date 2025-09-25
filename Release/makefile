################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/widgets/graph/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/widgets/canvas/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/widgets/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/containers/scrollers/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/containers/progress_indicators/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/containers/clock/subdir.mk
-include TouchGFX_App/touchgfx/framework/source/touchgfx/containers/subdir.mk
-include TouchGFX_App/gui/src/statistics_screen/subdir.mk
-include TouchGFX_App/gui/src/settings_screen/subdir.mk
-include TouchGFX_App/gui/src/model/subdir.mk
-include TouchGFX_App/gui/src/home_screen/subdir.mk
-include TouchGFX_App/gui/src/dashboard_screen/subdir.mk
-include TouchGFX_App/gui/src/containers/subdir.mk
-include TouchGFX_App/gui/src/common/subdir.mk
-include TouchGFX_App/generated/texts/src/subdir.mk
-include TouchGFX_App/generated/images/src/__generated/subdir.mk
-include TouchGFX_App/generated/images/src/subdir.mk
-include TouchGFX_App/generated/gui_generated/src/statistics_screen/subdir.mk
-include TouchGFX_App/generated/gui_generated/src/settings_screen/subdir.mk
-include TouchGFX_App/generated/gui_generated/src/home_screen/subdir.mk
-include TouchGFX_App/generated/gui_generated/src/dashboard_screen/subdir.mk
-include TouchGFX_App/generated/gui_generated/src/containers/subdir.mk
-include TouchGFX_App/generated/gui_generated/src/common/subdir.mk
-include TouchGFX_App/generated/fonts/src/subdir.mk
-include TouchGFX/target/generated/subdir.mk
-include TouchGFX/target/subdir.mk
-include TouchGFX/App/subdir.mk
-include Middlewares/Third_Party/LittleFs/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/RX8900SA/subdir.mk
-include Drivers/BSP/MX25L3233F/subdir.mk
-include Core/ThreadSafety/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include BSP/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := ENO_ROC
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ENO_ROC.elf \

MAP_FILES += \
ENO_ROC.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ENO_ROC.list \

OBJCOPY_BIN += \
ENO_ROC.bin \


# All Target
all: main-build

# Main-build Target
main-build: ENO_ROC.elf secondary-outputs

# Tool invocations
ENO_ROC.elf ENO_ROC.map: $(OBJS) $(USER_OBJS) /development/projects/EnoVation/enovation-bsp/ENO_ROC/STM32L4R7VITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "ENO_ROC.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/development/projects/EnoVation/enovation-bsp/ENO_ROC/STM32L4R7VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ENO_ROC.map" -Wl,--gc-sections -static -L../TouchGFX_App/touchgfx/lib/core/cortex_m4f/gcc --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ENO_ROC.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ENO_ROC.list"
	@echo 'Finished building: $@'
	@echo ' '

ENO_ROC.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ENO_ROC.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ENO_ROC.bin ENO_ROC.elf ENO_ROC.list ENO_ROC.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
