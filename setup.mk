<<<<<<< HEAD
### PROJECT_DIR 
### This is the path to where you have created/cloned your project
PROJECT_DIR       = $(PWD)
IOTIVITY_CONSTRAINED       = adapter/iotivity-constrained
### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(PROJECT_DIR)/Arduino-Makefile

### ARDUINO_DIR
### Path to the Arduino application and resources directory.
ARDUINO_DIR       = $(HOME)/arduino-home

### USER_LIB_PATH
### Path to where the your project's libraries are stored.
#USER_LIB_PATH    :=  $(PROJECT_DIR)/user_lib

### BOARD_TAG
### It must be set to the board you are currently using. (i.e uno, mega2560, etc.)
BOARD_TAG         = mega2560

### Variant Frequency
F_CPU			  = 16000000L
### Board Chip
MCU?=at$(BOARD_TAG)

### MCU ARCH
ARCHITECTURE=avr

#MCU VAriant
VARIANT						= mega
### FLASHING_BAUDRATE: todo detect the board tag to select the correct flash rate
AVRDUDE_ARD_BAUDRATE = 115200
### MONITOR_BAUDRATE
### It must be set to Serial baudrate value you are using.
MONITOR_BAUDRATE  = $(AVRDUDE_ARD_BAUDRATE)

### AVR_TOOLS_DIR
### Path to the AVR tools directory such as avr-gcc, avr-g++, etc.
AVR_TOOLS_DIR     = $(ARDUINO_DIR)/hardware/tools/avr
### or on Linux: (remove the one you don't want)

### AVRDUDE
AVRDUDE_ARD_PROGRAMMER = wiring

### ARDUINO LIBS
ARDUINO_LIBS +=  Wire SPI Time pRNG Ethernet2 

### CFLAGS_STD
### Set the C standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cflags_std)
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
### Set the C++ standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cxxflags_std)
CXXFLAGS_STD      += -std=gnu++11 

### CXXFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.#-pedantic
### from https://stackoverflow.com/questions/35586426/gcc-compiler-warning-flag-for-zero-variadic-macro-arguments
### -Wpedantic: emove warning: ISO C++11 requires at least one argument for the "..." in a variadic macro
### -Wvariadic-macros: warning: anonymous variadic macros were introduced in C++11
CXXFLAGS         += -Wno-attributes -Wno-variadic-macros -Wall -Wextra
### CFLAGS
CFLAGS           += -Wno-attributes -Wno-variadic-macros -Wall -Wextra

### If avr-gcc -v is higher than 4.9, activate coloring of the output
ifeq "$(AVR_GCC_VERSION)" "1"
    CXXFLAGS += -fdiagnostics-color
endif

### Library Options 
### Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

### Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

### If this is left blank, then it will use the Standard printf version.
PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)


### Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

### Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

### If this is left blank, then it will use the Standard scanf version.
SCANF_LIB = 
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

### External Memory Options 
## 64 KB of external RAM, starting after internal RAM (ATmega128!),
## used for variables (.data/.bss) and heap (malloc()).
##EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff
##EXTMEMOPTS = -Wl,--section-start,.data=0x802200,--defsym=__heap_end=0x80ffff
## 64 KB of external RAM, starting after internal RAM (ATmega128!),
## only used for heap (malloc()).
##  -Wl,--defsym=__heap_start=0x802200,--defsym=__heap_end=0x80ffff

ifeq ($(XMEM),1)    
     
#                         0x2200             0xffff    
# ---------------------------------------------------------
#|		|	<---- |	.data	 | .bss     | ---> |	 		|
#|		|	stack |variable  |	variable| heap |	 		|							
#|		|		  |			 |		    |      |     		|
# ----------------------------------------------------------
# use this config for static build: all data will be push to external ram
# might be useful for security build( consult .map file for info)
EXTMEMOPTS = -Wl,-Map,MegaXmem.map -Wl,--section-start,.data=0x802200,--defsym=__heap_end=0x80ffff,--defsym=__stack=0x8021ff
#                         0x802200             0x80ffff    
# -------------------------------------------------------
#| .data	|	.bss      |       |<---- |   |---> |	 |
#| variable	|	variables |       |stack |   |heap |	 |								
#|			|			  |		  |		 |	 |     |     |
# -------------------------------------------------------
# # Use this to push only dynamic allocation( heap) to external memory( this is the default usage)
##EXTMEMOPTS = -Wl,--defsym=__heap_start=0x802200,--defsym=__heap_end=0x80ffff

#                 0x8021ff 0x802200            0x80ffff    
# -----------------------------------------------
#| .bss		|      |<---- 	| .data  | ---> |	 |
#| variables|      | stack 	|variable| heap |	 |								
#|			|      |		|        |      |    |
# ------------------------------------------------
# this config can help with secuirty build as it push .bss in stack space(internal memory)
# and leaves .data( static or heap) to external memory( consult .map file for info)
##EXTMEMOPTS = -Wl,-Map,MegaDataXmem.map -Wl,--section-start,.bss=0x800200 -Wl,--section-start,.data=0x802200,--defsym=__heap_end=0x80ffff,--defsym=__stack=0x8021ff
## result of static build:
##AVR Memory Usage
##----------------
##Device: atmega2560
##
##Program:  240064 bytes (91.6% Full)
##(.text + .data + .bootloader)
##
##Data:      59490 bytes (726.2% Full)
##(.data + .bss + .noinit)
	
else
	EXTMEMOPTS =
endif

### Linker Options 
##  -Wl,...:   tell GCC to pass this to linker.
##  -Map:      create map file
##  --cref:    add cross reference to  map file
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) 


### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
###MONITOR_PORT   = /dev/ttyUSB*
MONITOR_PORT      = /dev/ttyACM*

### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))
=======
### PROJECT_DIR 
### This is the path to where you have created/cloned your project
PROJECT_DIR       = $(PWD)
IOTIVITY_CONSTRAINED       = adapter/iotivity-constrained
### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(PROJECT_DIR)/Arduino-Makefile

### ARDUINO_DIR
### Path to the Arduino application and resources directory.
ARDUINO_DIR       = $(HOME)/arduino-home

### AVRDUDE
AVRDUDE_ARD_PROGRAMMER = wiring

### ARDUINO LIBS
ifeq ($(ARCH),avr)
	ARDUINO_LIBS +=  Wire SPI Time pRNG Ethernet2 xmem SdFat Arduino-MemoryFree
else 
	ARDUINO_LIBS +=  Wire SPI Time Ethernet2 SdFat Arduino-MemoryFree
endif

### CFLAGS_STD
### Set the C standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cflags_std)
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
### Set the C++ standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cxxflags_std)
CXXFLAGS_STD      += -std=gnu++11 

### CXXFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.#-pedantic
### from https://stackoverflow.com/questions/35586426/gcc-compiler-warning-flag-for-zero-variadic-macro-arguments
### -Wpedantic: emove warning: ISO C++11 requires at least one argument for the "..." in a variadic macro
### -Wvariadic-macros: warning: anonymous variadic macros were introduced in C++11
CXXFLAGS         += -Wno-attributes -Wno-variadic-macros -Wall -Wextra
### CFLAGS
CFLAGS           += -Wno-attributes -Wno-variadic-macros -Wall -Wextra


### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))
>>>>>>> security
