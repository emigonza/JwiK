################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/arduino/uart/circbuff.c \
../platform/arduino/uart/uart.c 

OBJS += \
./platform/arduino/uart/circbuff.o \
./platform/arduino/uart/uart.o 

C_DEPS += \
./platform/arduino/uart/circbuff.d \
./platform/arduino/uart/uart.d 


# Each subdirectory must supply rules for building sources it contributes
platform/arduino/uart/%.o: ../platform/arduino/uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/opt/Java/workspaceJwiK/JwiK-VM" -I"/home/opt/Java/workspaceJwiK/JwiK-VM/platform/arduino" -I"/home/opt/Java/workspaceJwiK/JwiK-VM/platform/arduino/uart" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


