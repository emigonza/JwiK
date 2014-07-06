################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/arduino/Flash.c \
../platform/arduino/VmMain.c \
../platform/arduino/microapi.c \
../platform/arduino/native_brik.c \
../platform/arduino/native_formatter.c \
../platform/arduino/native_java.c \
../platform/arduino/native_math.c \
../platform/arduino/native_stdio.c \
../platform/arduino/platform.c \
../platform/arduino/uart.c 

OBJS += \
./platform/arduino/Flash.o \
./platform/arduino/VmMain.o \
./platform/arduino/microapi.o \
./platform/arduino/native_brik.o \
./platform/arduino/native_formatter.o \
./platform/arduino/native_java.o \
./platform/arduino/native_math.o \
./platform/arduino/native_stdio.o \
./platform/arduino/platform.o \
./platform/arduino/uart.o 

C_DEPS += \
./platform/arduino/Flash.d \
./platform/arduino/VmMain.d \
./platform/arduino/microapi.d \
./platform/arduino/native_brik.d \
./platform/arduino/native_formatter.d \
./platform/arduino/native_java.d \
./platform/arduino/native_math.d \
./platform/arduino/native_stdio.d \
./platform/arduino/platform.d \
./platform/arduino/uart.d 


# Each subdirectory must supply rules for building sources it contributes
platform/arduino/%.o: ../platform/arduino/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/opt/Java/workspaceJwiK/JwiK" -I"/home/opt/Java/workspaceJwiK/JwiK/platform/arduino" -I"/home/opt/Java/workspaceJwiK/JwiK/platform/arduino/uart" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


