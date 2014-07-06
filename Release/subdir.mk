################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../native_string.c \
../utils.c \
../vm.c \
../vmarray.c \
../vmerror.c \
../vmfile.c \
../vmheap.c \
../vmstack.c 

OBJS += \
./native_string.o \
./utils.o \
./vm.o \
./vmarray.o \
./vmerror.o \
./vmfile.o \
./vmheap.o \
./vmstack.o 

C_DEPS += \
./native_string.d \
./utils.d \
./vm.d \
./vmarray.d \
./vmerror.d \
./vmfile.d \
./vmheap.d \
./vmstack.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/opt/Java/workspaceJwiK/JwiK" -I"/home/opt/Java/workspaceJwiK/JwiK/platform/arduino" -I"/home/opt/Java/workspaceJwiK/JwiK/platform/arduino/uart" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


