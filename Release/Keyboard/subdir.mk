################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Keyboard/keyboard.c 

OBJS += \
./Keyboard/keyboard.o 

C_DEPS += \
./Keyboard/keyboard.d 


# Each subdirectory must supply rules for building sources it contributes
Keyboard/%.o Keyboard/%.su: ../Keyboard/%.c Keyboard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"F:/stm32f401ccu6_filemanager/Display" -I"F:/stm32f401ccu6_filemanager/MyString" -I"F:/stm32f401ccu6_filemanager/Keyboard" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Keyboard

clean-Keyboard:
	-$(RM) ./Keyboard/keyboard.d ./Keyboard/keyboard.o ./Keyboard/keyboard.su

.PHONY: clean-Keyboard

