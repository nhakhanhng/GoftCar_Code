################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utils/SyncPrintf/SyncPrintf.c 

OBJS += \
./Utils/SyncPrintf/SyncPrintf.o 

C_DEPS += \
./Utils/SyncPrintf/SyncPrintf.d 


# Each subdirectory must supply rules for building sources it contributes
Utils/SyncPrintf/%.o Utils/SyncPrintf/%.su Utils/SyncPrintf/%.cyclo: ../Utils/SyncPrintf/%.c Utils/SyncPrintf/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"D:/Project/GolfCartV2/SteeringNode/Code/Periodics" -I"D:/Project/GolfCartV2/SteeringNode/Code/STM32_RTOS_PeripheralDriver" -I"D:/Project/GolfCartV2/SteeringNode/Code/STM32-Hardware-Driver" -I"D:/Project/GolfCartV2/SteeringNode/Code/Utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Utils-2f-SyncPrintf

clean-Utils-2f-SyncPrintf:
	-$(RM) ./Utils/SyncPrintf/SyncPrintf.cyclo ./Utils/SyncPrintf/SyncPrintf.d ./Utils/SyncPrintf/SyncPrintf.o ./Utils/SyncPrintf/SyncPrintf.su

.PHONY: clean-Utils-2f-SyncPrintf

