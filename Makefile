include ./setup.mk

ifeq ($(ARCH),avr)
	PRNG_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-prng.a
endif
SERVER_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-adapter.a
TIME_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-time.a
WIZ5500_ARCHIVE = build-$(BOARD_TAG)/libarduino-wiz5500.a
SERIAL_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-serial.a
SDFAT_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-sdfat.a



ifeq ($(ARCH),avr)
ifeq ($(SERVER),1)
  CXXFLAGS 			+= -DOC_SERVER
ifneq ($(XMEM),1)
  LOCAL_CPP_SRCS 	+= apps/server/server.cpp
else
  CXXFLAGS 		    += -DOC_XMEM
  LOCAL_CPP_SRCS 	+= apps/server/server-xmem.cpp	
endif
else ifeq ($(CLIENT),1)
  CXXFLAGS 			+= -DOC_CLIENT
ifneq ($(XMEM),1)	
  LOCAL_CPP_SRCS 	+= apps/client/client.cpp
else
  LOCAL_CPP_SRCS 	+= apps/client/client-xmem.cpp
endif		
else
$(error Must define an app server/client to proceed!)	
endif
else
# ARM
ifeq ($(SERVER),1)
  CXXFLAGS 			+= -DOC_SERVER
  LOCAL_CPP_SRCS 	+= apps/server/server.cpp
else ifeq ($(CLIENT),1)
  CXXFLAGS 			+= -DOC_CLIENT
  LOCAL_CPP_SRCS 	+= apps/client/client.cpp
else
#$(error Target Architecture must be define to proceed!)	
$(error Must define an app server/client to proceed!)	
endif
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
ifeq ($(ARCH),avr)
	DEPS_HEADERS      +=$(addprefix -I$(PROJECT_DIR)/deps/, pRNG wiz5500 serial sdFat)
else
	DEPS_HEADERS      +=$(addprefix -I$(PROJECT_DIR)/deps/, wiz5500 serial sdFat)
endif
STACK_HEADERS     +=$(addprefix -I$(IOTIVITY_CONSTRAINED)/, . messaging/coap util util/pt include  api port security deps/mbedtls/include/mbedtls)
ADAPTER_HEADERS   +=$(addprefix -I$(PROJECT_DIR)/adapter/ipadapter/, include include/wiznet_inc)
APPS_HEADERS      +=-I$(PROJECT_DIR)/apps/include
CORE_HEADERS      +=-I$(ARDUINO_DIR)/libraries/SdFat/src/FatLib
CXXFLAGS          += $(CORE_HEADERS) $(ADAPTER_HEADERS) $(STACK_HEADERS) $(DEPS_HEADERS) $(APPS_HEADERS)


ifeq ($(SECURE),1)
	SEC_HEADERS	+=$(addprefix -I$(IOTIVITY_CONSTRAINED)/deps/mbedtls/, include include/mbedtls)
	CXXFLAGS		+=$(SEC_HEADERS)
	CXXFLAGS		+= -DOC_SEC
endif

SERVER_OBJ     = $(PROJECT_DIR)/adapter/$(SERVER_ARCHIVE)
OTHER_OBJS    += $(SERVER_OBJ)

ifeq ($(ARCH),avr)
OTHER_OBJS 		+= $(PROJECT_DIR)/deps/pRNG/$(PRNG_ARCHIVE)	
endif
OTHER_OBJS 		+= $(PROJECT_DIR)/deps/Time/$(TIME_ARCHIVE)
OTHER_OBJS 		+= $(PROJECT_DIR)/deps/wiz5500/$(WIZ5500_ARCHIVE)
OTHER_OBJS 		+= $(PROJECT_DIR)/deps/serial/$(SERIAL_ARCHIVE)	
OTHER_OBJS 		+= $(PROJECT_DIR)/deps/sdFat/$(SDFAT_ARCHIVE)

#OTHER_LIBS  	+= $(DEPS_LIBS) 

ifeq ($(ARCH),avr)
	include ./avr.mk
else ifeq ($(ARCH),samd)
	include ./samd.mk
else
$(error Target Architecture must be define to proceed!)	
endif

ifeq ($(ARCH),avr)
$(SERVER_OBJ): PRNG_ARCHIVE TIME_ARCHIVE WIZ5500_ARCHIVE SDFAT_ARCHIVE SERIAL_ARCHIVE 
	$(MAKE) -C ./adapter $(SERVER_ARCHIVE)
else
$(SERVER_OBJ): TIME_ARCHIVE WIZ5500_ARCHIVE SDFAT_ARCHIVE SERIAL_ARCHIVE 
	$(MAKE) -C ./adapter $(SERVER_ARCHIVE)
endif

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
ifeq ($(ARCH),avr)
	$(MAKE) -C ./deps/pRNG clean  
endif
	$(MAKE) -C ./deps/Time clean
	$(MAKE) -C ./deps/wiz5500 clean
	$(MAKE) -C ./deps/serial clean
	$(MAKE) -C ./deps/sdFat clean
