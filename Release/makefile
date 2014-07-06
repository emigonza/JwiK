################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include platform/arduino/uart/subdir.mk
-include platform/arduino/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
JwiK-VM.lss \

FLASH_IMAGE += \
JwiK-VM.hex \

EEPROM_IMAGE += \
JwiK-VM.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: JwiK-VM.elf secondary-outputs

# Tool invocations
JwiK-VM.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,JwiK-VM.map -mmcu=atmega328p -o "JwiK-VM.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

JwiK-VM.lss: JwiK-VM.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S JwiK-VM.elf  >"JwiK-VM.lss"
	@echo 'Finished building: $@'
	@echo ' '

JwiK-VM.hex: JwiK-VM.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex JwiK-VM.elf  "JwiK-VM.hex"
	@echo 'Finished building: $@'
	@echo ' '

JwiK-VM.eep: JwiK-VM.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex JwiK-VM.elf  "JwiK-VM.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: JwiK-VM.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t JwiK-VM.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) JwiK-VM.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
