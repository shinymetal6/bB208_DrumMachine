################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DrumMachine/Hmi/fonts.c \
../Core/Src/DrumMachine/Hmi/graph.c \
../Core/Src/DrumMachine/Hmi/menus.c \
../Core/Src/DrumMachine/Hmi/vol_meter.c 

OBJS += \
./Core/Src/DrumMachine/Hmi/fonts.o \
./Core/Src/DrumMachine/Hmi/graph.o \
./Core/Src/DrumMachine/Hmi/menus.o \
./Core/Src/DrumMachine/Hmi/vol_meter.o 

C_DEPS += \
./Core/Src/DrumMachine/Hmi/fonts.d \
./Core/Src/DrumMachine/Hmi/graph.d \
./Core/Src/DrumMachine/Hmi/menus.d \
./Core/Src/DrumMachine/Hmi/vol_meter.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DrumMachine/Hmi/%.o: ../Core/Src/DrumMachine/Hmi/%.c Core/Src/DrumMachine/Hmi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Core/Src/bB2xx_Bsp -I../Core/Src/DrumMachine -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DrumMachine-2f-Hmi

clean-Core-2f-Src-2f-DrumMachine-2f-Hmi:
	-$(RM) ./Core/Src/DrumMachine/Hmi/fonts.d ./Core/Src/DrumMachine/Hmi/fonts.o ./Core/Src/DrumMachine/Hmi/graph.d ./Core/Src/DrumMachine/Hmi/graph.o ./Core/Src/DrumMachine/Hmi/menus.d ./Core/Src/DrumMachine/Hmi/menus.o ./Core/Src/DrumMachine/Hmi/vol_meter.d ./Core/Src/DrumMachine/Hmi/vol_meter.o

.PHONY: clean-Core-2f-Src-2f-DrumMachine-2f-Hmi

