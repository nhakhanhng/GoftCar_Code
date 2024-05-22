################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.c 

OBJS += \
./STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.o 

C_DEPS += \
./STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_RTOS_PeripheralDriver/SyncPrintf/%.o STM32_RTOS_PeripheralDriver/SyncPrintf/%.su STM32_RTOS_PeripheralDriver/SyncPrintf/%.cyclo: ../STM32_RTOS_PeripheralDriver/SyncPrintf/%.c STM32_RTOS_PeripheralDriver/SyncPrintf/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/steering-node/Code/Periodics" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/steering-node/Code/STM32_RTOS_PeripheralDriver" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/steering-node/Code/STM32-Hardware-Driver" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/steering-node/Code/UtilitySmallFunction" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/steering-node/Code/CANlibrary" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32_RTOS_PeripheralDriver-2f-SyncPrintf

clean-STM32_RTOS_PeripheralDriver-2f-SyncPrintf:
	-$(RM) ./STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.cyclo ./STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.d ./STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.o ./STM32_RTOS_PeripheralDriver/SyncPrintf/SyncPrintf.su

.PHONY: clean-STM32_RTOS_PeripheralDriver-2f-SyncPrintf

