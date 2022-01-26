################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DrumMachine/Drum/bpm.c \
../Core/Src/DrumMachine/Drum/delay.c \
../Core/Src/DrumMachine/Drum/drum.c \
../Core/Src/DrumMachine/Drum/drum_buffers.c \
../Core/Src/DrumMachine/Drum/drum_flash.c \
../Core/Src/DrumMachine/Drum/enc_btns_leds.c \
../Core/Src/DrumMachine/Drum/gate.c \
../Core/Src/DrumMachine/Drum/instrument.c \
../Core/Src/DrumMachine/Drum/sdcard.c \
../Core/Src/DrumMachine/Drum/sequencer.c \
../Core/Src/DrumMachine/Drum/trigger.c 

OBJS += \
./Core/Src/DrumMachine/Drum/bpm.o \
./Core/Src/DrumMachine/Drum/delay.o \
./Core/Src/DrumMachine/Drum/drum.o \
./Core/Src/DrumMachine/Drum/drum_buffers.o \
./Core/Src/DrumMachine/Drum/drum_flash.o \
./Core/Src/DrumMachine/Drum/enc_btns_leds.o \
./Core/Src/DrumMachine/Drum/gate.o \
./Core/Src/DrumMachine/Drum/instrument.o \
./Core/Src/DrumMachine/Drum/sdcard.o \
./Core/Src/DrumMachine/Drum/sequencer.o \
./Core/Src/DrumMachine/Drum/trigger.o 

C_DEPS += \
./Core/Src/DrumMachine/Drum/bpm.d \
./Core/Src/DrumMachine/Drum/delay.d \
./Core/Src/DrumMachine/Drum/drum.d \
./Core/Src/DrumMachine/Drum/drum_buffers.d \
./Core/Src/DrumMachine/Drum/drum_flash.d \
./Core/Src/DrumMachine/Drum/enc_btns_leds.d \
./Core/Src/DrumMachine/Drum/gate.d \
./Core/Src/DrumMachine/Drum/instrument.d \
./Core/Src/DrumMachine/Drum/sdcard.d \
./Core/Src/DrumMachine/Drum/sequencer.d \
./Core/Src/DrumMachine/Drum/trigger.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DrumMachine/Drum/%.o: ../Core/Src/DrumMachine/Drum/%.c Core/Src/DrumMachine/Drum/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Core/Src/bB2xx_Bsp -I../Core/Src/DrumMachine -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DrumMachine-2f-Drum

clean-Core-2f-Src-2f-DrumMachine-2f-Drum:
	-$(RM) ./Core/Src/DrumMachine/Drum/bpm.d ./Core/Src/DrumMachine/Drum/bpm.o ./Core/Src/DrumMachine/Drum/delay.d ./Core/Src/DrumMachine/Drum/delay.o ./Core/Src/DrumMachine/Drum/drum.d ./Core/Src/DrumMachine/Drum/drum.o ./Core/Src/DrumMachine/Drum/drum_buffers.d ./Core/Src/DrumMachine/Drum/drum_buffers.o ./Core/Src/DrumMachine/Drum/drum_flash.d ./Core/Src/DrumMachine/Drum/drum_flash.o ./Core/Src/DrumMachine/Drum/enc_btns_leds.d ./Core/Src/DrumMachine/Drum/enc_btns_leds.o ./Core/Src/DrumMachine/Drum/gate.d ./Core/Src/DrumMachine/Drum/gate.o ./Core/Src/DrumMachine/Drum/instrument.d ./Core/Src/DrumMachine/Drum/instrument.o ./Core/Src/DrumMachine/Drum/sdcard.d ./Core/Src/DrumMachine/Drum/sdcard.o ./Core/Src/DrumMachine/Drum/sequencer.d ./Core/Src/DrumMachine/Drum/sequencer.o ./Core/Src/DrumMachine/Drum/trigger.d ./Core/Src/DrumMachine/Drum/trigger.o

.PHONY: clean-Core-2f-Src-2f-DrumMachine-2f-Drum

