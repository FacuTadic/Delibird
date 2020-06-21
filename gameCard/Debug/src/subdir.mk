################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/archivosYDirectorios.c \
../src/creacionDeEstructurasParaEnvio.c \
../src/gameCard.c \
../src/protocoloGameCard.c \
../src/utilsGameCard.c 

OBJS += \
./src/archivosYDirectorios.o \
./src/creacionDeEstructurasParaEnvio.o \
./src/gameCard.o \
./src/protocoloGameCard.o \
./src/utilsGameCard.o 

C_DEPS += \
./src/archivosYDirectorios.d \
./src/creacionDeEstructurasParaEnvio.d \
./src/gameCard.d \
./src/protocoloGameCard.d \
./src/utilsGameCard.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


