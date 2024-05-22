################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32-Hardware-Driver/Encoder/Encoder.c 

OBJS += \
./STM32-Hardware-Driver/Encoder/Encoder.o 

C_DEPS += \
./STM32-Hardware-Driver/Encoder/Encoder.d 


# Each subdirectory must supply rules for building sources it contributes
STM32-Hardware-Driver/Encoder/%.o STM32-Hardware-Driver/Encoder/%.su STM32-Hardware-Driver/Encoder/%.cyclo: ../STM32-Hardware-Driver/Encoder/%.c STM32-Hardware-Driver/Encoder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32_RTOS_PeripheralDriver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32-Hardware-Driver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Periodics" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Utils" -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/vanti/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.5/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32-2d-Hardware-2d-Driver-2f-Encoder

clean-STM32-2d-Hardware-2d-Driver-2f-Encoder:
	-$(RM) ./STM32-Hardware-Driver/Encoder/Encoder.cyclo ./STM32-Hardware-Driver/Encoder/Encoder.d ./STM32-Hardware-Driver/Encoder/Encoder.o ./STM32-Hardware-Driver/Encoder/Encoder.su

.PHONY: clean-STM32-2d-Hardware-2d-Driver-2f-Encoder

