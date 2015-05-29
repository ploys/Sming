################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := del

# All of the sources participating in the build are defined here
-include sources.mk
-include out/build/app/subdir.mk
-include out/Sming/system/subdir.mk
-include out/Sming/appinit/subdir.mk
-include out/Sming/Wiring/subdir.mk
-include out/Sming/SmingCore/Platform/subdir.mk
-include out/Sming/SmingCore/Network/subdir.mk
-include out/Sming/SmingCore/subdir.mk
-include out/Sming/Services/WebHelpers/subdir.mk
-include out/Sming/Services/SpifFS/subdir.mk
-include out/Sming/Services/DateTime/subdir.mk
-include out/Sming/Services/ArduinoJson/subdir.mk
-include out/Sming/Libraries/Ultrasonic/subdir.mk
-include app/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Ultrasonic_HCSR04

# Tool invocations
Ultrasonic_HCSR04: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C++ Linker'
	g++  -o "Ultrasonic_HCSR04" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(C_DEPS) Ultrasonic_HCSR04
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
