################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include UtilitySmallFunction/Utility/subdir.mk
-include UtilitySmallFunction/DataProcess/subdir.mk
-include UtilitySmallFunction/CRC/subdir.mk
-include STM32_RTOS_PeripheralDriver/isotp-c/subdir.mk
-include STM32_RTOS_PeripheralDriver/UARTHandler/subdir.mk
-include STM32_RTOS_PeripheralDriver/SyncPrintf/subdir.mk
-include STM32_RTOS_PeripheralDriver/I2CHandler/subdir.mk
-include STM32_RTOS_PeripheralDriver/GPIO/subdir.mk
-include STM32_RTOS_PeripheralDriver/CANHandler/subdir.mk
-include STM32_RTOS_PeripheralDriver/CANFrame/subdir.mk
-include STM32-Hardware-Driver/XKC_IR/subdir.mk
-include STM32-Hardware-Driver/VL53L0X/platform/Src/subdir.mk
-include STM32-Hardware-Driver/VL53L0X/core/Src/subdir.mk
-include STM32-Hardware-Driver/VL53L0X/Vl53l0xHandler/subdir.mk
-include STM32-Hardware-Driver/LEDSpeakerControl/subdir.mk
-include STM32-Hardware-Driver/INA219/subdir.mk
-include STM32-Hardware-Driver/GPSHandler/lwgps/subdir.mk
-include STM32-Hardware-Driver/GPSHandler/subdir.mk
-include Periods/PowerPublisher/subdir.mk
-include Periods/GPSPublisher/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := LightGPSNode
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
LightGPSNode.elf \

MAP_FILES += \
LightGPSNode.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LightGPSNode.list \


# All Target
all: main-build

# Main-build Target
main-build: LightGPSNode.elf secondary-outputs

# Tool invocations
LightGPSNode.elf LightGPSNode.map: $(OBJS) $(USER_OBJS) C:\Users\nhakh\Desktop\Document\STM\GolfCartV2-Documentation\Code\LightGPSNode\Code\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "LightGPSNode.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\nhakh\Desktop\Document\STM\GolfCartV2-Documentation\Code\LightGPSNode\Code\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="LightGPSNode.map" -Wl,--gc-sections -static -L"C:\Users\nhakh\Desktop\Document\STM\GolfCartV2-Documentation\Code\LightGPSNode\Code\STM32_RTOS_PeripheralDriver" --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LightGPSNode.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LightGPSNode.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) LightGPSNode.elf LightGPSNode.list LightGPSNode.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
