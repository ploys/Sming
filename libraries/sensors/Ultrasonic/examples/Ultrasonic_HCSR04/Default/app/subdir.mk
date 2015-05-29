################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app/application.cpp 

OBJS += \
./app/application.o 

CPP_DEPS += \
./app/application.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o: ../app/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Espressif\ESP8266_SDK\include\" -I../../Sming -I../../Sming/Wiring -I../../Sming/SmingCore -I../../Sming/SmingCore/Network -I../../Sming/SmingCore/Platform -I../../Sming/system/include -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


