################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Game-watch-server/servidor.c \
../src/Game-watch-server/utils.c 

OBJS += \
./src/Game-watch-server/servidor.o \
./src/Game-watch-server/utils.o 

C_DEPS += \
./src/Game-watch-server/servidor.d \
./src/Game-watch-server/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/Game-watch-server/%.o: ../src/Game-watch-server/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


