################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/broker.c \
../src/broker_ids.c \
../src/memoria_broker.c \
../src/protocolBroker.c \
../src/utilsBroker.c 

OBJS += \
./src/broker.o \
./src/broker_ids.o \
./src/memoria_broker.o \
./src/protocolBroker.o \
./src/utilsBroker.o 

C_DEPS += \
./src/broker.d \
./src/broker_ids.d \
./src/memoria_broker.d \
./src/protocolBroker.d \
./src/utilsBroker.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


