################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bB2xx_Bsp/Core/audio_buffers.c \
../Core/Src/bB2xx_Bsp/Core/bB2xx_System.c \
../Core/Src/bB2xx_Bsp/Core/bB2xx_flash.c \
../Core/Src/bB2xx_Bsp/Core/control_buffer.c \
../Core/Src/bB2xx_Bsp/Core/irq_manager.c 

OBJS += \
./Core/Src/bB2xx_Bsp/Core/audio_buffers.o \
./Core/Src/bB2xx_Bsp/Core/bB2xx_System.o \
./Core/Src/bB2xx_Bsp/Core/bB2xx_flash.o \
./Core/Src/bB2xx_Bsp/Core/control_buffer.o \
./Core/Src/bB2xx_Bsp/Core/irq_manager.o 

C_DEPS += \
./Core/Src/bB2xx_Bsp/Core/audio_buffers.d \
./Core/Src/bB2xx_Bsp/Core/bB2xx_System.d \
./Core/Src/bB2xx_Bsp/Core/bB2xx_flash.d \
./Core/Src/bB2xx_Bsp/Core/control_buffer.d \
./Core/Src/bB2xx_Bsp/Core/irq_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bB2xx_Bsp/Core/%.o: ../Core/Src/bB2xx_Bsp/Core/%.c Core/Src/bB2xx_Bsp/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Core/Src/bB2xx_Bsp -I../Core/Src/DrumMachine -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-bB2xx_Bsp-2f-Core

clean-Core-2f-Src-2f-bB2xx_Bsp-2f-Core:
	-$(RM) ./Core/Src/bB2xx_Bsp/Core/audio_buffers.d ./Core/Src/bB2xx_Bsp/Core/audio_buffers.o ./Core/Src/bB2xx_Bsp/Core/bB2xx_System.d ./Core/Src/bB2xx_Bsp/Core/bB2xx_System.o ./Core/Src/bB2xx_Bsp/Core/bB2xx_flash.d ./Core/Src/bB2xx_Bsp/Core/bB2xx_flash.o ./Core/Src/bB2xx_Bsp/Core/control_buffer.d ./Core/Src/bB2xx_Bsp/Core/control_buffer.o ./Core/Src/bB2xx_Bsp/Core/irq_manager.d ./Core/Src/bB2xx_Bsp/Core/irq_manager.o

.PHONY: clean-Core-2f-Src-2f-bB2xx_Bsp-2f-Core

