###iotivity contrained sources
include ../../setup.mk 

#ifndef USART_BAUD
USART_BAUD	= 115200
#endif

ARDUINO_MODEL =  Mega

# http://arduino.cc/en/Main/Boards
ifeq ($(ARDUINO_MODEL), Diecimila) # ATmega168 @ 16MHz
# http://www.arduino.cc/en/Main/ArduinoBoardDiecimila
AVR_MCU_CLOCK	= 16
AVR_MCU_SPEED	= 16000000UL
MCU		= atmega168
USART_PORT	= 0

else ifeq ($(ARDUINO_MODEL), ProMini8MHz) # ATmega168 @ 8MHz (3.3V)
# http://www.arduino.cc/en/Main/ArduinoBoardProMini
AVR_MCU_CLOCK	= 8
AVR_MCU_SPEED	= 8000000UL
MCU		= atmega168
USART_PORT	= 0

else ifeq ($(ARDUINO_MODEL), ProMini16MHz) # ATmega168 @ 16MHz (5V)
# http://www.arduino.cc/en/Main/ArduinoBoardProMini
AVR_MCU_CLOCK	= 16
AVR_MCU_SPEED	= 16000000UL
MCU		= atmega168
USART_PORT	= 0

else ifeq ($(ARDUINO_MODEL), Duemilanove-328) # ATmega328p @ 16MHz (5V)
# http://arduino.cc/en/Main/ArduinoBoardDuemilanove
AVR_MCU_CLOCK	= 16
AVR_MCU_SPEED	= 16000000UL
MCU		= atmega328p
USART_PORT	= 0

else ifeq ($(ARDUINO_MODEL), Uno) # ATmega328p @ 16MHz (5V)
# http://arduino.cc/en/Main/ArduinoBoardUno
AVR_MCU_CLOCK	= 16
AVR_MCU_SPEED	= 16000000UL
MCU		= atmega328p 
USART_PORT	= 0

else ifeq ($(ARDUINO_MODEL), Mega) # ATmega2560 @ 16MHz (5V)
# http://arduino.cc/en/Main/ArduinoBoardUno
AVR_MCU_CLOCK	= 16
AVR_MCU_SPEED	= 16000000UL
MCU		= atmega2560 
USART_PORT	= 0

else
$(error Please define Arduino board model in Makefile.arduino)
endif


RS232_PLAT_DEFS	= -DF_CPU=$(AVR_MCU_SPEED) \
			  -DMCU_MHZ=$(AVR_MCU_CLOCK) \
			  -DAUTO_CRC_PADDING=2 \
			  -DUSART_BAUD=USART_BAUD_$(USART_BAUD) \
			  -DUSART_PORT=RS232_PORT_$(USART_PORT)

#CFLAGS += $(RS232_PLAT_DEFS)

SRC=$(wildcard ./*.c) 

LOCAL_C_SRCS += $(SRC) 

include $(ARDMK_DIR)/Arduino.mk 

build-$(BOARD_TAG)/libarduino-rs232.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS)				