################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32-Hardware-Driver/EngineControl/EngineControl.c 

OBJS += \
./STM32-Hardware-Driver/EngineControl/EngineControl.o 

C_DEPS += \
./STM32-Hardware-Driver/EngineControl/EngineControl.d 


# Each subdirectory must supply rules for building sources it contributes
STM32-Hardware-Driver/EngineControl/%.o STM32-Hardware-Driver/EngineControl/%.su STM32-Hardware-Driver/EngineControl/%.cyclo: ../STM32-Hardware-Driver/EngineControl/%.c STM32-Hardware-Driver/EngineControl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"D:/Project/GolfCartV2/SteeringNode/Code/Periodics" -I"D:/Project/GolfCartV2/SteeringNode/Code/STM32_RTOS_PeripheralDriver" -I"D:/Project/GolfCartV2/SteeringNode/Code/STM32-Hardware-Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32-2d-Hardware-2d-Driver-2f-EngineControl

clean-STM32-2d-Hardware-2d-Driver-2f-EngineControl:
	-$(RM) ./STM32-Hardware-Driver/EngineControl/EngineControl.cyclo ./STM32-Hardware-Driver/EngineControl/EngineControl.d ./STM32-Hardware-Driver/EngineControl/EngineControl.o ./STM32-Hardware-Driver/EngineControl/EngineControl.su

.PHONY: clean-STM32-2d-Hardware-2d-Driver-2f-EngineControl

