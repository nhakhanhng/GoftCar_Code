################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.c 

OBJS += \
./STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.o 

C_DEPS += \
./STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_RTOS_PeripheralDriver/I2CHandler/%.o STM32_RTOS_PeripheralDriver/I2CHandler/%.su STM32_RTOS_PeripheralDriver/I2CHandler/%.cyclo: ../STM32_RTOS_PeripheralDriver/I2CHandler/%.c STM32_RTOS_PeripheralDriver/I2CHandler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/STM32_RTOS_PeripheralDriver" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/STM32-Hardware-Driver" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/Periods" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/UtilitySmallFunction" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/STM32-Hardware-Driver/GPSHandler/lwgps" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32_RTOS_PeripheralDriver-2f-I2CHandler

clean-STM32_RTOS_PeripheralDriver-2f-I2CHandler:
	-$(RM) ./STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.cyclo ./STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.d ./STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.o ./STM32_RTOS_PeripheralDriver/I2CHandler/I2C_Handler.su

.PHONY: clean-STM32_RTOS_PeripheralDriver-2f-I2CHandler

