# This file was automagically generated by mbed.org. For more information, 
# see http://mbed.org/handbook/Exporting-to-GCC-ARM-Embedded

GCC_BIN = 
PROJECT = ak
OBJECTS = ./main.o ./AKController.o ./mbed-rtos/rtx/TARGET_M3/TOOLCHAIN_GCC/SVC_Table.o ./mbed-rtos/rtx/TARGET_M3/TOOLCHAIN_GCC/HAL_CM3.o ./mbed-rtos/rtx/rt_Task.o ./mbed-rtos/rtx/rt_System.o ./mbed-rtos/rtx/rt_List.o ./mbed-rtos/rtx/rt_Time.o ./mbed-rtos/rtx/rt_Semaphore.o ./mbed-rtos/rtx/rt_Mailbox.o ./mbed-rtos/rtx/rt_CMSIS.o ./mbed-rtos/rtx/HAL_CM.o ./mbed-rtos/rtx/rt_Robin.o ./mbed-rtos/rtx/rt_MemBox.o ./mbed-rtos/rtx/RTX_Conf_CM.o ./mbed-rtos/rtx/rt_Event.o ./mbed-rtos/rtx/rt_Mutex.o ./mbed-rtos/rtos/Thread.o ./mbed-rtos/rtos/RtosTimer.o ./mbed-rtos/rtos/Semaphore.o ./mbed-rtos/rtos/Mutex.o ./m3pi/m3pi.o 
SYS_OBJECTS = ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/cmsis_nvic.o ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/system_LPC17xx.o ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/startup_LPC17xx.o ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/retarget.o 
INCLUDE_PATHS = -I. -I./m3pi -I./mbed-rtos -I./mbed-rtos/rtx -I./mbed-rtos/rtx/TARGET_M3 -I./mbed-rtos/rtx/TARGET_M3/TOOLCHAIN_GCC -I./mbed-rtos/rtos -I./m3pi -I./mbed -I./mbed/TARGET_LPC1768 -I./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM 

LIBRARY_PATHS = -L./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM 
LIBRARIES = -lmbed 
LINKER_SCRIPT = ./mbed/TARGET_LPC1768/TOOLCHAIN_GCC_ARM/LPC1768.ld

############################################################################### 
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy

CPU = -mcpu=cortex-m3 -mthumb
CC_FLAGS = $(CPU) -c -Os -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections 
CC_SYMBOLS = -DTARGET_LPC1768 -DTARGET_M3 -DTARGET_NXP -DTARGET_LPC176X -DTOOLCHAIN_GCC_ARM -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 

LD_FLAGS = -mcpu=cortex-m3 -mthumb -Wl,--gc-sections --specs=nano.specs -u _printf_float -u _scanf_float
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

all: $(PROJECT).bin

clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(OBJECTS)

.s.o:
	$(AS) $(CPU) -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(LIBRARIES) $(LD_SYS_LIBS) $(LIBRARIES) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
