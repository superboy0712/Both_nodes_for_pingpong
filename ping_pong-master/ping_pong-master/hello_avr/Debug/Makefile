################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../oled_demo


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../oled_demo/ADC.c \
../oled_demo/CAN.c \
../oled_demo/iA_PWM_NODE_1.c \
../oled_demo/main.c \
../oled_demo/MicroMenu.c \
../oled_demo/oled.c \
../oled_demo/pwmmusic.c \
../oled_demo/usart.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
oled_demo/ADC.o \
oled_demo/CAN.o \
oled_demo/iA_PWM_NODE_1.o \
oled_demo/main.o \
oled_demo/MicroMenu.o \
oled_demo/oled.o \
oled_demo/pwmmusic.o \
oled_demo/usart.o

OBJS_AS_ARGS +=  \
oled_demo/ADC.o \
oled_demo/CAN.o \
oled_demo/iA_PWM_NODE_1.o \
oled_demo/main.o \
oled_demo/MicroMenu.o \
oled_demo/oled.o \
oled_demo/pwmmusic.o \
oled_demo/usart.o

C_DEPS +=  \
oled_demo/ADC.d \
oled_demo/CAN.d \
oled_demo/iA_PWM_NODE_1.d \
oled_demo/main.d \
oled_demo/MicroMenu.d \
oled_demo/oled.d \
oled_demo/pwmmusic.d \
oled_demo/usart.d

C_DEPS_AS_ARGS +=  \
oled_demo/ADC.d \
oled_demo/CAN.d \
oled_demo/iA_PWM_NODE_1.d \
oled_demo/main.d \
oled_demo/MicroMenu.d \
oled_demo/oled.d \
oled_demo/pwmmusic.d \
oled_demo/usart.d

OUTPUT_FILE_PATH +=hello_avr.elf

OUTPUT_FILE_PATH_AS_ARGS +=hello_avr.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler

















oled_demo/%.o: ../oled_demo/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega162 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="hello_avr.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega162  
	@echo Finished building target: $@
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "hello_avr.elf" "hello_avr.hex"
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "hello_avr.elf" "hello_avr.eep" || exit 0
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "hello_avr.elf" > "hello_avr.lss"
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "hello_avr.elf" "hello_avr.srec"
	"C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "hello_avr.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "hello_avr.elf" "hello_avr.a" "hello_avr.hex" "hello_avr.lss" "hello_avr.eep" "hello_avr.map" "hello_avr.srec" "hello_avr.usersignatures"
	