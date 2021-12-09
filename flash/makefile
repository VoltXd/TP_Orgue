################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: TP_Orgue.axf

# Tool invocations
TP_Orgue.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Andre\Desktop\Etudes\ENS\Cours\M1 - E3A\S1\441 - Informatique Industrielle\TP\TP5\peripherals_lib\flash" -L"C:\Users\Andre\Desktop\Etudes\ENS\Cours\M1 - E3A\S1\441 - Informatique Industrielle\TP\TP5\utilities_lib\flash" -Xlinker -Map="TP_Orgue.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T "TP3_Timer_flash.ld" -o "TP_Orgue.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) TP_Orgue.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TP_Orgue.axf"; # arm-none-eabi-objcopy -v -O binary "TP_Orgue.axf" "TP_Orgue.bin" ; # checksum -p LPC804 -d "TP_Orgue.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
