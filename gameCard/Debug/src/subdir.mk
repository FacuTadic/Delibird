################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/archivosYDirectorios.c \
../src/gameCard.c \
../src/utilsGameCard.c 

OBJS += \
./src/archivosYDirectorios.o \
./src/gameCard.o \
./src/utilsGameCard.o 

C_DEPS += \
./src/archivosYDirectorios.d \
./src/gameCard.d \
./src/utilsGameCard.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


