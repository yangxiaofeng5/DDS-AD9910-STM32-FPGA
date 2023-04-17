################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/COMMON.c \
../src/main1.c \
../src/platform.c 

OBJS += \
./src/COMMON.o \
./src/main1.o \
./src/platform.o 

C_DEPS += \
./src/COMMON.d \
./src/main1.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../AD9910-DDS-PS_bsp/ps7_cortexa9_0/include -I"E:\Program_Station\Embedded_Program\FPGA\AD9910-DDS-PS\AD9910-DDS-PS.sdk\AD9910-DDS-PS\ACZ702_Lib\AD9910" -I"E:\Program_Station\Embedded_Program\FPGA\AD9910-DDS-PS\AD9910-DDS-PS.sdk\AD9910-DDS-PS\ACZ702_Lib\PS_GPIO" -I"E:\Program_Station\Embedded_Program\FPGA\AD9910-DDS-PS\AD9910-DDS-PS.sdk\AD9910-DDS-PS\src" -I"E:\Program_Station\Embedded_Program\FPGA\AD9910-DDS-PS\AD9910-DDS-PS.sdk\AD9910-DDS-PS\ACZ702_Lib\PS_UART" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


