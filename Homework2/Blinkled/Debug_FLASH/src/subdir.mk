################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Application_BlinkLed.c \
../src/PCC.c \
../src/Port.c \
../src/main.c 

OBJS += \
./src/Application_BlinkLed.o \
./src/PCC.o \
./src/Port.o \
./src/main.o 

C_DEPS += \
./src/Application_BlinkLed.d \
./src/PCC.d \
./src/Port.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Application_BlinkLed.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


