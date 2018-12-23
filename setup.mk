### DISCLAIMER
### This is an example Makefile and it MUST be configured to suit your needs.
### For detailed explanations about all of the available options, please refer
### to https://github.com/sudar/Arduino-Makefile/blob/master/arduino-mk-vars.md
### Original project where this Makefile comes from: https://github.com/WeAreLeka/Bare-Arduino-Project

### PROJECT_DIR 02229912
### This is the path to where you have created/cloned your project
PROJECT_DIR       = $(HOME)/arduino-iotivity
IOTIVITY_CONSTRAINED       = $(PROJECT_DIR)/components/adapter/iotivity-constrained
### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(PROJECT_DIR)/arduino-mk

### ARDUINO_DIR
### Path to the Arduino application and resources directory.
ARDUINO_DIR       = $(PROJECT_DIR)/arduino-home


### USER_LIB_PATH
### Path to where the your project's libraries are stored.
##USER_LIB_PATH    :=  $(PROJECT_DIR)/lib

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
ARDUINO_LIBS +=  Wire SPI Time pRNG 

### CFLAGS_STD
### Set the C standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cflags_std)
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
### Set the C++ standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cxxflags_std)
CXXFLAGS_STD      = -std=gnu++11

### CXXFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.
CXXFLAGS         += -pedantic -Wall -Wextra

### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT      = /dev/ttyACM*

### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))

### OBJDIR
### This is where you put the binaries you just compile using 'make'
###OBJDIR            = $(PROJECT_DIR)/bin/$(BOARD_TAG)/$(CURRENT_DIR)
##CC_NAME=avr-gcc

### Iotivity contrained includes
PORT_INC        = $(IOTIVITY_CONSTRAINED)/port
CORE_INC        = $(IOTIVITY_CONSTRAINED)/include
ADAPTER_INC     = $(PROJECT_DIR)/components/adapter/include
COAP_INC        = $(IOTIVITY_CONSTRAINED)/messaging/coap
UTIL_INC        = $(IOTIVITY_CONSTRAINED)/util
UTIL_PT_INC     = $(IOTIVITY_CONSTRAINED)/util/pt/
MATHER_INC      = $(PROJECT_DIR)/components/wrappers/mather
SERIAL_INC      = $(PROJECT_DIR)/components/wrappers/IotivitySerial
PRNG_INC        = $(PROJECT_DIR)/components/wrappers/pRNG

CFLAGS   += -I$(IOTIVITY_CONSTRAINED) -I$(COAP_INC) -I$(UTIL_INC) -I$(UTIL_PT_INC) -I$(PORT_INC) -I$(ADAPTER_INC) -I$(CORE_INC) -I$(PORT_INC) -I$(MATHER_INC) -I$(SERIAL_INC) -I$(PRNG_INC)
CXXFLAGS += -I$(IOTIVITY_CONSTRAINED) -I$(COAP_INC) -I$(UTIL_INC) -I$(UTIL_PT_INC) -I$(PORT_INC) -I$(ADAPTER_INC) -I$(CORE_INC) -I$(PORT_INC) -I$(MATHER_INC) -I$(SERIAL_INC) -I$(PRNG_INC)