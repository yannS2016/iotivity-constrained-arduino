include ./setup.mk
SERVER_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-adapter.a
PRNG_ARCHIVE 		= build-$(BOARD_TAG)/libarduino-prng.a
TIME_ARCHIVE 		= build-$(BOARD_TAG)/libarduino-time.a
WIZ5500_ARCHIVE = build-$(BOARD_TAG)/libarduino-wiz5500.a
RS232_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-rs232.a
SERIAL_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-serial.a
SDFAT_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-sdfat.a

ifeq ($(SERVER),1)
  CXXFLAGS 				+= -DOC_SERVER 
ifeq ($(XMEM),1)
	CXXFLAGS += -DOC_XMEM
  LOCAL_CPP_SRCS 	+= apps/server/server-xmem.cpp
else
  LOCAL_CPP_SRCS 	+= apps/server/server.cpp
endif
endif

ifeq ($(CLIENT),1)
  LOCAL_CPP_SRCS 		+= apps/client/client.cpp
  CXXFLAGS 					+= -DOC_CLIENT
endif

ifeq ($(DYNAMIC),1)
	CXXFLAGS += -DOC_DYNAMIC_ALLOCATION
endif

ifeq ($(XMEM),1)
	CXXFLAGS += -DOC_XMEM
endif

ifeq ($(IPV4),1)
	CXXFLAGS += -DOC_IPV4
endif

ifeq ($(ESP32),1)
	CXXFLAGS += -DOC_ESP32
endif

### Iotivity contrained includes
STACK_HEADERS     +=$(addprefix -I$(IOTIVITY_CONSTRAINED)/, . messaging/coap util util/pt include  api port security deps/mbedtls/include/mbedtls)
DEPS_HEADERS      +=$(addprefix -I$(PROJECT_DIR)/deps/, pRNG wiz5500 serial sdFat)
ADAPTER_HEADERS   +=-I$(PROJECT_DIR)/adapter/ipadapter/include 
ADAPTER_HEADERS   +=-I$(PROJECT_DIR)/adapter/ipadapter/include/wiznet_inc
#CORE_HEADERS      += # -I$(ARDUINO_DIR)/libraries/Time -I$(ARDUINO_DIR)/SdFat/src/FatLib
CORE_HEADERS      +=-I$(ARDUINO_DIR)/libraries/Time -I$(ARDUINO_DIR)/libraries/SdFat/src/FatLib
APPS_HEADERS      +=-I$(PROJECT_DIR)/apps/include
CXXFLAGS          += $(CORE_HEADERS) $(ADAPTER_HEADERS) $(STACK_HEADERS) $(DEPS_HEADERS) $(APPS_HEADERS)


ifeq ($(SECURE),1)
	SEC_HEADERS	+=$(addprefix -I$(IOTIVITY_CONSTRAINED)/deps/mbedtls/, include include/mbedtls)
	CXXFLAGS		+=$(SEC_HEADERS)
	CXXFLAGS		+= -DOC_SEC
endif

SERVER_OBJ     = $(PROJECT_DIR)/adapter/$(SERVER_ARCHIVE)
OTHER_OBJS    += $(SERVER_OBJ)

DEPS_LIBS 		+= $(PROJECT_DIR)/deps/pRNG/$(PRNG_ARCHIVE)	
DEPS_LIBS 		+= $(PROJECT_DIR)/deps/Time/$(TIME_ARCHIVE)
DEPS_LIBS 		+= $(PROJECT_DIR)/deps/wiz5500/$(WIZ5500_ARCHIVE)
DEPS_LIBS 		+= $(PROJECT_DIR)/deps/serial/$(SERIAL_ARCHIVE)	
DEPS_LIBS 		+= $(PROJECT_DIR)/deps/sdFat/$(SDFAT_ARCHIVE)
OTHER_LIBS  	+= $(DEPS_LIBS) 


include $(ARDMK_DIR)/Arduino.mk

$(SERVER_OBJ): PRNG_ARCHIVE TIME_ARCHIVE WIZ5500_ARCHIVE  SDFAT_ARCHIVE SERIAL_ARCHIVE 
	$(MAKE) -C ./adapter $(SERVER_ARCHIVE)

PRNG_ARCHIVE: 
	$(MAKE) -C ./deps/pRNG $(PRNG_ARCHIVE)	

TIME_ARCHIVE: 
	$(MAKE) -C ./deps/Time $(TIME_ARCHIVE)	
	
WIZ5500_ARCHIVE: 
	$(MAKE) -C ./deps/wiz5500 $(WIZ5500_ARCHIVE)	

SERIAL_ARCHIVE : 
	$(MAKE) -C ./deps/serial $(SERIAL_ARCHIVE)  

SDFAT_ARCHIVE: 
	$(MAKE) -C ./deps/sdFat $(SDFAT_ARCHIVE)  

clean::
	$(MAKE) -C ./adapter clean
	$(MAKE) -C ./deps/pRNG clean  
	$(MAKE) -C ./deps/Time clean
	$(MAKE) -C ./deps/wiz5500 clean
	$(MAKE) -C ./deps/serial clean
	$(MAKE) -C ./deps/sdFat clean
