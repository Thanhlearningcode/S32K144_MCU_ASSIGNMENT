################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DesignPattern/Builder/PCC_builder.c \
../src/DesignPattern/Builder/Port_builder.c 

OBJS += \
./src/DesignPattern/Builder/PCC_builder.o \
./src/DesignPattern/Builder/Port_builder.o 

C_DEPS += \
./src/DesignPattern/Builder/PCC_builder.d \
./src/DesignPattern/Builder/Port_builder.d 


# Each subdirectory must supply rules for building sources it contributes
src/DesignPattern/Builder/%.o: ../src/DesignPattern/Builder/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/DesignPattern/Builder/PCC_builder.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


