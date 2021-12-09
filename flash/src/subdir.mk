################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ADC_orgue.c \
../src/CTIMER_orgue.c \
../src/DAC_orgue.c \
../src/GPIO_orgue.c \
../src/I2C_orgue.c \
../src/MCUXpresso_cr_startup.c \
../src/MCUXpresso_crp.c \
../src/MCUXpresso_mtb.c \
../src/MRT_orgue.c \
../src/UART_orgue.c \
../src/lib_ENS_II1_lcd.c \
../src/music_auto.c \
../src/orgueElectronique.c \
../src/system.c \
../src/toolbox.c 

OBJS += \
./src/ADC_orgue.o \
./src/CTIMER_orgue.o \
./src/DAC_orgue.o \
./src/GPIO_orgue.o \
./src/I2C_orgue.o \
./src/MCUXpresso_cr_startup.o \
./src/MCUXpresso_crp.o \
./src/MCUXpresso_mtb.o \
./src/MRT_orgue.o \
./src/UART_orgue.o \
./src/lib_ENS_II1_lcd.o \
./src/music_auto.o \
./src/orgueElectronique.o \
./src/system.o \
./src/toolbox.o 

C_DEPS += \
./src/ADC_orgue.d \
./src/CTIMER_orgue.d \
./src/DAC_orgue.d \
./src/GPIO_orgue.d \
./src/I2C_orgue.d \
./src/MCUXpresso_cr_startup.d \
./src/MCUXpresso_crp.d \
./src/MCUXpresso_mtb.d \
./src/MRT_orgue.d \
./src/UART_orgue.d \
./src/lib_ENS_II1_lcd.d \
./src/music_auto.d \
./src/orgueElectronique.d \
./src/system.d \
./src/toolbox.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DCR_INTEGER_PRINTF -D__USE_CMSIS -D__CODE_RED -DCORE_M0PLUS -D__MTB_DISABLE -D__MTB_BUFFER_SIZE=256 -D__REDLIB__ -I"C:\Users\Andre\Desktop\Etudes\ENS\Cours\M1 - E3A\S1\441 - Informatique Industrielle\TP\TP5\TP_Orgue\inc" -I"C:\Users\Andre\Desktop\Etudes\ENS\Cours\M1 - E3A\S1\441 - Informatique Industrielle\TP\TP5\peripherals_lib\inc" -I"C:\Users\Andre\Desktop\Etudes\ENS\Cours\M1 - E3A\S1\441 - Informatique Industrielle\TP\TP5\utilities_lib\inc" -I"C:\Users\Andre\Desktop\Etudes\ENS\Cours\M1 - E3A\S1\441 - Informatique Industrielle\TP\TP5\common\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


