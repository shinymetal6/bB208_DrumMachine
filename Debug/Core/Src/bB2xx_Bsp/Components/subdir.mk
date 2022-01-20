################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bB2xx_Bsp/Components/adc.c \
../Core/Src/bB2xx_Bsp/Components/dac.c \
../Core/Src/bB2xx_Bsp/Components/h_interfaces.c \
../Core/Src/bB2xx_Bsp/Components/i2s.c \
../Core/Src/bB2xx_Bsp/Components/ili9341.c \
../Core/Src/bB2xx_Bsp/Components/ili9341_calibrate_touch.c \
../Core/Src/bB2xx_Bsp/Components/ili9341_touch.c \
../Core/Src/bB2xx_Bsp/Components/sine_wave.c \
../Core/Src/bB2xx_Bsp/Components/spdif.c \
../Core/Src/bB2xx_Bsp/Components/w25qxx.c 

OBJS += \
./Core/Src/bB2xx_Bsp/Components/adc.o \
./Core/Src/bB2xx_Bsp/Components/dac.o \
./Core/Src/bB2xx_Bsp/Components/h_interfaces.o \
./Core/Src/bB2xx_Bsp/Components/i2s.o \
./Core/Src/bB2xx_Bsp/Components/ili9341.o \
./Core/Src/bB2xx_Bsp/Components/ili9341_calibrate_touch.o \
./Core/Src/bB2xx_Bsp/Components/ili9341_touch.o \
./Core/Src/bB2xx_Bsp/Components/sine_wave.o \
./Core/Src/bB2xx_Bsp/Components/spdif.o \
./Core/Src/bB2xx_Bsp/Components/w25qxx.o 

C_DEPS += \
./Core/Src/bB2xx_Bsp/Components/adc.d \
./Core/Src/bB2xx_Bsp/Components/dac.d \
./Core/Src/bB2xx_Bsp/Components/h_interfaces.d \
./Core/Src/bB2xx_Bsp/Components/i2s.d \
./Core/Src/bB2xx_Bsp/Components/ili9341.d \
./Core/Src/bB2xx_Bsp/Components/ili9341_calibrate_touch.d \
./Core/Src/bB2xx_Bsp/Components/ili9341_touch.d \
./Core/Src/bB2xx_Bsp/Components/sine_wave.d \
./Core/Src/bB2xx_Bsp/Components/spdif.d \
./Core/Src/bB2xx_Bsp/Components/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bB2xx_Bsp/Components/%.o: ../Core/Src/bB2xx_Bsp/Components/%.c Core/Src/bB2xx_Bsp/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Core/Src/bB2xx_Bsp -I../Core/Src/DrumMachine -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-bB2xx_Bsp-2f-Components

clean-Core-2f-Src-2f-bB2xx_Bsp-2f-Components:
	-$(RM) ./Core/Src/bB2xx_Bsp/Components/adc.d ./Core/Src/bB2xx_Bsp/Components/adc.o ./Core/Src/bB2xx_Bsp/Components/dac.d ./Core/Src/bB2xx_Bsp/Components/dac.o ./Core/Src/bB2xx_Bsp/Components/h_interfaces.d ./Core/Src/bB2xx_Bsp/Components/h_interfaces.o ./Core/Src/bB2xx_Bsp/Components/i2s.d ./Core/Src/bB2xx_Bsp/Components/i2s.o ./Core/Src/bB2xx_Bsp/Components/ili9341.d ./Core/Src/bB2xx_Bsp/Components/ili9341.o ./Core/Src/bB2xx_Bsp/Components/ili9341_calibrate_touch.d ./Core/Src/bB2xx_Bsp/Components/ili9341_calibrate_touch.o ./Core/Src/bB2xx_Bsp/Components/ili9341_touch.d ./Core/Src/bB2xx_Bsp/Components/ili9341_touch.o ./Core/Src/bB2xx_Bsp/Components/sine_wave.d ./Core/Src/bB2xx_Bsp/Components/sine_wave.o ./Core/Src/bB2xx_Bsp/Components/spdif.d ./Core/Src/bB2xx_Bsp/Components/spdif.o ./Core/Src/bB2xx_Bsp/Components/w25qxx.d ./Core/Src/bB2xx_Bsp/Components/w25qxx.o

.PHONY: clean-Core-2f-Src-2f-bB2xx_Bsp-2f-Components

