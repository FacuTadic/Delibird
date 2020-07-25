################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/GameBoy.c \
../src/protocoloGameBoy.c \
../src/utilsGameBoy.c 

OBJS += \
./src/GameBoy.o \
./src/protocoloGameBoy.o \
./src/utilsGameBoy.o 

C_DEPS += \
./src/GameBoy.d \
./src/protocoloGameBoy.d \
./src/utilsGameBoy.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


