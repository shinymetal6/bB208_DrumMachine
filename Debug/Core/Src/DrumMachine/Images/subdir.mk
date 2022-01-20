################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DrumMachine/Images/blue_digits.c \
../Core/Src/DrumMachine/Images/digits_memory.c \
../Core/Src/DrumMachine/Images/green_digits.c \
../Core/Src/DrumMachine/Images/orange_digits.c \
../Core/Src/DrumMachine/Images/red_digits.c \
../Core/Src/DrumMachine/Images/yellow_digits.c 

OBJS += \
./Core/Src/DrumMachine/Images/blue_digits.o \
./Core/Src/DrumMachine/Images/digits_memory.o \
./Core/Src/DrumMachine/Images/green_digits.o \
./Core/Src/DrumMachine/Images/orange_digits.o \
./Core/Src/DrumMachine/Images/red_digits.o \
./Core/Src/DrumMachine/Images/yellow_digits.o 

C_DEPS += \
./Core/Src/DrumMachine/Images/blue_digits.d \
./Core/Src/DrumMachine/Images/digits_memory.d \
./Core/Src/DrumMachine/Images/green_digits.d \
./Core/Src/DrumMachine/Images/orange_digits.d \
./Core/Src/DrumMachine/Images/red_digits.d \
./Core/Src/DrumMachine/Images/yellow_digits.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DrumMachine/Images/%.o: ../Core/Src/DrumMachine/Images/%.c Core/Src/DrumMachine/Images/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Core/Src/bB2xx_Bsp -I../Core/Src/DrumMachine -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DrumMachine-2f-Images

clean-Core-2f-Src-2f-DrumMachine-2f-Images:
	-$(RM) ./Core/Src/DrumMachine/Images/blue_digits.d ./Core/Src/DrumMachine/Images/blue_digits.o ./Core/Src/DrumMachine/Images/digits_memory.d ./Core/Src/DrumMachine/Images/digits_memory.o ./Core/Src/DrumMachine/Images/green_digits.d ./Core/Src/DrumMachine/Images/green_digits.o ./Core/Src/DrumMachine/Images/orange_digits.d ./Core/Src/DrumMachine/Images/orange_digits.o ./Core/Src/DrumMachine/Images/red_digits.d ./Core/Src/DrumMachine/Images/red_digits.o ./Core/Src/DrumMachine/Images/yellow_digits.d ./Core/Src/DrumMachine/Images/yellow_digits.o

.PHONY: clean-Core-2f-Src-2f-DrumMachine-2f-Images

