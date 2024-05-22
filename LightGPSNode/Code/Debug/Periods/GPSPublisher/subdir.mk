################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Periods/GPSPublisher/GPSPublisher.c 

OBJS += \
./Periods/GPSPublisher/GPSPublisher.o 

C_DEPS += \
./Periods/GPSPublisher/GPSPublisher.d 


# Each subdirectory must supply rules for building sources it contributes
Periods/GPSPublisher/%.o Periods/GPSPublisher/%.su Periods/GPSPublisher/%.cyclo: ../Periods/GPSPublisher/%.c Periods/GPSPublisher/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/STM32_RTOS_PeripheralDriver" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/STM32-Hardware-Driver" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/Periods" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/UtilitySmallFunction" -I"C:/Users/nhakh/Desktop/Document/STM/GolfCartV2-Documentation/Code/LightGPSNode/Code/STM32-Hardware-Driver/GPSHandler/lwgps" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Periods-2f-GPSPublisher

clean-Periods-2f-GPSPublisher:
	-$(RM) ./Periods/GPSPublisher/GPSPublisher.cyclo ./Periods/GPSPublisher/GPSPublisher.d ./Periods/GPSPublisher/GPSPublisher.o ./Periods/GPSPublisher/GPSPublisher.su

.PHONY: clean-Periods-2f-GPSPublisher

