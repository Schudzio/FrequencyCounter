################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32F1xx-Nucleo/stm32f1xx_nucleo.c 

OBJS += \
./Utilities/STM32F1xx-Nucleo/stm32f1xx_nucleo.o 

C_DEPS += \
./Utilities/STM32F1xx-Nucleo/stm32f1xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32F1xx-Nucleo/%.o: ../Utilities/STM32F1xx-Nucleo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DNUCLEO_F103RB -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"C:/Users/lotus/workspace/licznik/Utilities/STM32F1xx-Nucleo" -I"C:/Users/lotus/workspace/licznik/StdPeriph_Driver/inc" -I"C:/Users/lotus/workspace/licznik/inc" -I"C:/Users/lotus/workspace/licznik/CMSIS/device" -I"C:/Users/lotus/workspace/licznik/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


