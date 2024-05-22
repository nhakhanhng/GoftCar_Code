################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.c \
../STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.c \
../STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.c \
../STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.c \
../STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.c \
../STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.c 

OBJS += \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.o \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.o \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.o \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.o \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.o \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.o 

C_DEPS += \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.d \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.d \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.d \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.d \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.d \
./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.d 


# Each subdirectory must supply rules for building sources it contributes
STM32-Hardware-Driver/GPSHandler/lwgps/examples/%.o STM32-Hardware-Driver/GPSHandler/lwgps/examples/%.su STM32-Hardware-Driver/GPSHandler/lwgps/examples/%.cyclo: ../STM32-Hardware-Driver/GPSHandler/lwgps/examples/%.c STM32-Hardware-Driver/GPSHandler/lwgps/examples/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32_RTOS_PeripheralDriver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/STM32-Hardware-Driver" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Periodics" -I"D:/Project/GolfCartV2/LightGPSNode/Code/Utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32-2d-Hardware-2d-Driver-2f-GPSHandler-2f-lwgps-2f-examples

clean-STM32-2d-Hardware-2d-Driver-2f-GPSHandler-2f-lwgps-2f-examples:
	-$(RM) ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.d ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.o ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example.su ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.d ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.o ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_buff.su ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.d ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.o ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_dist_bear.su ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.d ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.o ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/example_stat.su ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.d ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.o ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_code.su ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.cyclo ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.d ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.o ./STM32-Hardware-Driver/GPSHandler/lwgps/examples/test_time.su

.PHONY: clean-STM32-2d-Hardware-2d-Driver-2f-GPSHandler-2f-lwgps-2f-examples

