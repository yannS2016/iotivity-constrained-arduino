### PROJECT_DIR 
### This is the path to where you have created/cloned your project
PROJECT_DIR       = $(PWD)
IOTIVITY_CONSTRAINED       = adapter/iotivity-constrained
### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(HOME)/arduino-mk

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
### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT      = /dev/ttyACM*

### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))
