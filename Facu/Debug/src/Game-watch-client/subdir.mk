################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Game-watch-client/tp0.c \
../src/Game-watch-client/utils.c 

O_SRCS += \
../src/Game-watch-client/tp0.o \
../src/Game-watch-client/utils.o 

OBJS += \
./src/Game-watch-client/tp0.o \
./src/Game-watch-client/utils.o 

C_DEPS += \
./src/Game-watch-client/tp0.d \
./src/Game-watch-client/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/Game-watch-client/%.o: ../src/Game-watch-client/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


