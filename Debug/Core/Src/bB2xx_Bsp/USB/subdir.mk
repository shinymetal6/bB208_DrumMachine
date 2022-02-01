################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bB2xx_Bsp/USB/usbd_desc.c 

OBJS += \
./Core/Src/bB2xx_Bsp/USB/usbd_desc.o 

C_DEPS += \
./Core/Src/bB2xx_Bsp/USB/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bB2xx_Bsp/USB/%.o: ../Core/Src/bB2xx_Bsp/USB/%.c Core/Src/bB2xx_Bsp/USB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Core/Src/bB2xx_Bsp -I../Core/Src/bB2xx_Bsp/MIDI -I../Core/Src/DrumMachine -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-bB2xx_Bsp-2f-USB

clean-Core-2f-Src-2f-bB2xx_Bsp-2f-USB:
	-$(RM) ./Core/Src/bB2xx_Bsp/USB/usbd_desc.d ./Core/Src/bB2xx_Bsp/USB/usbd_desc.o

.PHONY: clean-Core-2f-Src-2f-bB2xx_Bsp-2f-USB

