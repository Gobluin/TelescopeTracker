################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../i2c_lib/I2CRoutines.c \
../i2c_lib/i2c_interupts.c \
../i2c_lib/main.c 

OBJS += \
./i2c_lib/I2CRoutines.o \
./i2c_lib/i2c_interupts.o \
./i2c_lib/main.o 

C_DEPS += \
./i2c_lib/I2CRoutines.d \
./i2c_lib/i2c_interupts.d \
./i2c_lib/main.d 


# Each subdirectory must supply rules for building sources it contributes
i2c_lib/%.o: ../i2c_lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


