###iotivity contrained sources
include ../../setup.mk

DTLS += $(addprefix ../iotivity-constrained/deps/tinydtls/, ccm.c hmac.c netq.c peer.c \
						dtls_time.c session.c sha2/sha2.c ecc/ecc.c aes/rijndael.c crypto.c dtls.c)
DTLSFLAGS=-DDTLSV12 -DWITH_SHA256 -DDTLS_CHECK_CONTENTTYPE -DWITH_OCF  -DNDEBUG

CBOR+=$(addprefix ../iotivity-constrained/deps/tinycbor/src/, cborencoder.c \
																		cborencoder_close_container_checked.c cborparser.c)

SRC_COMMON=$(wildcard  ../iotivity-constrained/util/*.c) $(CBOR)
SRC=$(wildcard ../iotivity-constrained/messaging/coap/*.c ../iotivity-constrained/api/*.c ./src/*.c )

WZNET_SRC=$(wildcard ./src/wiznet_src/*.c)
SRC+=$(WZNET_SRC)

LOCAL_C_SRCS += $(SRC_COMMON)
LOCAL_C_SRCS += $(SRC) 

# Arduino ethernet and iotivity constrained includes
ADPTER_HEADERS+=-I./include -I./include/wiznet_inc
TIME_HEADERS=-I$(ARDUINO_DIR)/libraries/Time
ETH2_UTILS=-I$(ARDUINO_DIR)/libraries/Ethernet2/src/utility
WRAPPERS_HEADERS+=$(addprefix -I../deps/, pRNG rs232 wiz5500)
HEADERS_STACK+=$(addprefix -I../iotivity-constrained/, messaging/coap util util/pt include  api port .)
CFLAGS +=$(ADPTER_HEADERS) $(ETH2_UTILS) $(TIME_HEADERS) $(HEADERS_STACK) $(WRAPPERS_HEADERS)

ifeq ($(DEBUG),1)
	CFLAGS += -DOC_DEBUG 
else
	CFLAGS += -Wl,--gc-sections,-Wno-attributes
	
endif

ifeq ($(DYNAMIC),1)
	CFLAGS += -DOC_DYNAMIC_ALLOCATION
endif

ifeq ($(SECURE),1)
	SRC += $(wildcard  ../iotivity-constrained/security/*.c)
	SRC_COMMON += $(DTLS)
	HEADERS_DTLS = $(IOTIVITY_CONSTRAINED_DIR)/deps/tinydtls
    HEADERS_SEC = $(IOTIVITY_CONSTRAINED_DIR)/deps/mbedtls/library
	CFLAGS += $(DTLSFLAGS) -DOC_SECURITY -I$(HEADERS_DTLS) I$(HEADERS_SEC) 
	
endif

ifeq ($(IPV4),1)
	CFLAGS += -DOC_IPV4
endif


ifeq ($(SERVER),1)
	CFLAGS 	+= -DOC_SERVER
endif

ifeq ($(CLIENT),1)
	CFLAGS 	+= -DOC_CLIENT
endif


include $(ARDMK_DIR)/Arduino.mk
	
build-$(BOARD_TAG)/libiotivity-constrained.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS) 