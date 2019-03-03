###iotivity contrained sources
include ../setup.mk

DTLS += $(addprefix iotivity-constrained/deps/tinydtls/, ccm.c hmac.c netq.c peer.c \
						dtls_time.c session.c sha2/sha2.c ecc/ecc.c aes/rijndael.c crypto.c dtls.c)
DTLSFLAGS=-DDTLSV12 -DWITH_SHA256 -DDTLS_CHECK_CONTENTTYPE -DWITH_OCF  -DNDEBUG

CBOR+=$(addprefix iotivity-constrained/deps/tinycbor/src/, cborencoder.c \
																		cborencoder_close_container_checked.c cborparser.c)

SRC_COMMON=$(wildcard  iotivity-constrained/util/*.c) $(CBOR)
SRC=$(wildcard iotivity-constrained/messaging/coap/*.c iotivity-constrained/api/*.c ipadapter/src/*.c )

WZNET_SRC=$(wildcard ipadapter/src/wiznet_src/*.c)
SRC+=$(WZNET_SRC)

LOCAL_C_SRCS += $(SRC_COMMON)
LOCAL_C_SRCS += $(SRC) 

# Arduino ethernet and iotivity constrained includes
ADPTER_HEADERS+=-Iipadapter/include -Iipadapter/include/wiznet_inc
ETH2_UTILS=-I$(ARDUINO_DIR)/libraries/Ethernet2/src/utility
WRAPPERS_HEADERS+=$(addprefix -I../deps/, pRNG rs232 wiz5500)
HEADERS_STACK+=$(addprefix -Iiotivity-constrained/, messaging/coap util util/pt include  api port .)
CFLAGS +=$(ADPTER_HEADERS) $(ETH2_UTILS) $(HEADERS_STACK) $(WRAPPERS_HEADERS)

ifeq ($(DEBUG),1)
	CFLAGS += -DOC_DEBUG 
else
	CFLAGS += -Wl,--gc-sections,-Wno-attributes
	
endif

ifeq ($(DYNAMIC),1)
	CFLAGS += -DOC_DYNAMIC_ALLOCATION
endif

ifeq ($(SECURE),1)
	
endif

ifeq ($(IPV4),1)
	CFLAGS += -DOC_IPV4
endif
# activate this if you have an xmem shield added to your arduino board
ifeq ($(XMEM),1)
	CFLAGS += -DOC_XMEM
endif


ifeq ($(SERVER),1)
	CFLAGS 	+= -DOC_SERVER
endif

ifeq ($(CLIENT),1)
	CFLAGS 	+= -DOC_CLIENT
endif


include $(ARDMK_DIR)/Arduino.mk
	
build-$(BOARD_TAG)/libarduino-adapter.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS) 