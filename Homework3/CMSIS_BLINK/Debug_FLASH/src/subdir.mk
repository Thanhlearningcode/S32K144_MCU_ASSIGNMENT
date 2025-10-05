################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gpio_driver.c \
../src/main.c \
../src/port_driver.c 

OBJS += \
./src/gpio_driver.o \
./src/main.o \
./src/port_driver.o 

C_DEPS += \
./src/gpio_driver.d \
./src/main.d \
./src/port_driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/gpio_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


