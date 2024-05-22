################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.c 

OBJS += \
./STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.o 

C_DEPS += \
./STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.d 


# Each subdirectory must supply rules for building sources it contributes
STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/%.o STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/%.su STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/%.cyclo: ../STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/%.c STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32_RTOS_PeripheralDriver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32-Hardware-Driver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Periodics" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32-2d-Hardware-2d-Driver-2f-GPSHandler-2f-lwgps-2f-lwgps-2f-src-2f-lwgps

clean-STM32-2d-Hardware-2d-Driver-2f-GPSHandler-2f-lwgps-2f-lwgps-2f-src-2f-lwgps:
	-$(RM) ./STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.d ./STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.o ./STM32-Hardware-Driver/GPSHandler/lwgps/lwgps/src/lwgps/lwgps.su

.PHONY: clean-STM32-2d-Hardware-2d-Driver-2f-GPSHandler-2f-lwgps-2f-lwgps-2f-src-2f-lwgps

