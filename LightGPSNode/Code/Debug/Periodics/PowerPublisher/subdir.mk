################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Periodics/PowerPublisher/PowerPublisher.c 

OBJS += \
./Periodics/PowerPublisher/PowerPublisher.o 

C_DEPS += \
./Periodics/PowerPublisher/PowerPublisher.d 


# Each subdirectory must supply rules for building sources it contributes
Periodics/PowerPublisher/%.o Periodics/PowerPublisher/%.su Periodics/PowerPublisher/%.cyclo: ../Periodics/PowerPublisher/%.c Periodics/PowerPublisher/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32_RTOS_PeripheralDriver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32-Hardware-Driver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Periodics" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Utils" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Periodics-2f-PowerPublisher

clean-Periodics-2f-PowerPublisher:
	-$(RM) ./Periodics/PowerPublisher/PowerPublisher.cyclo ./Periodics/PowerPublisher/PowerPublisher.d ./Periodics/PowerPublisher/PowerPublisher.o ./Periodics/PowerPublisher/PowerPublisher.su

.PHONY: clean-Periodics-2f-PowerPublisher

