###iotivity contrained sources
include ../setup.mk

DTLS += $(addprefix iotivity-constrained/deps/mbedtls/mbedtls/library/, aes.c		aesni.c 	arc4.c  	asn1parse.c	asn1write.c	base64.c	\
	bignum.c	blowfish.c	camellia.c	ccm.c		cipher.c	cipher_wrap.c	\
	cmac.c		ctr_drbg.c	des.c		dhm.c		ecdh.c		ecdsa.c		\
	ecjpake.c	ecp.c		ecp_curves.c	entropy.c	entropy_poll.c	error.c		\
	gcm.c		havege.c	hmac_drbg.c	md.c		md2.c		md4.c		\
	md5.c		md_wrap.c	oid.c		padlock.c	\
	pem.c		pk.c		pk_wrap.c	pkcs12.c	pkcs5.c		pkparse.c	\
	pkwrite.c	platform.c	ripemd160.c	rsa.c		sha1.c		sha256.c	\
	sha512.c	threading.c	timing.c	version.c	version_features.c		\
	xtea.c  	pkcs11.c 	x509.c 		x509_crt.c	debug.c		net_sockets.c	\
	ssl_cache.c	ssl_ciphersuites.c		ssl_cli.c	ssl_cookie.c			\
	ssl_srv.c	ssl_ticket.c	ssl_tls.c	rsa_internal.c	x509write_csr.c\
	x509write_crt.c	x509_create.c )
 
DTLSFLAGS=$(addprefix -Iiotivity-constrained/, security deps/mbedtls/mbedtls/include) -D__OC_RANDOM

CBOR+=$(addprefix iotivity-constrained/deps/tinycbor/src/, cborencoder.c \
																		cborencoder_close_container_checked.c cborparser.c)

SRC_COMMON=$(wildcard  iotivity-constrained/util/*.c) $(CBOR)
SRC=$(wildcard iotivity-constrained/messaging/coap/*.c iotivity-constrained/api/*.c ipadapter/src/*.c )

WZNET_SRC=$(wildcard ipadapter/src/wiznet_src/*.c)
SRC+=$(WZNET_SRC)


# Arduino ethernet and iotivity constrained includes
ADPTER_HEADERS+=-Iipadapter/include -Iipadapter/include/wiznet_inc
CORE_HEADERS=-I$(ARDUINO_DIR)/libraries/Ethernet2/src/utility -I$(ARDUINO_DIR)/libraries/SdFat/src/FatLib
WRAPPERS_HEADERS+=$(addprefix -I../deps/, pRNG rs232 wiz5500 sdFat serial)
HEADERS_STACK+=$(addprefix -Iiotivity-constrained/, messaging/coap util util/pt include  api port .)
CFLAGS +=$(ADPTER_HEADERS) $(CORE_HEADERS) $(HEADERS_STACK) $(WRAPPERS_HEADERS)

ifeq ($(DEBUG),1)
	CFLAGS += -DOC_DEBUG 
else
	CFLAGS += -Wl,--gc-sections,-Wno-attributes
	
endif

ifeq ($(DYNAMIC),1)
	CFLAGS += -DOC_DYNAMIC_ALLOCATION
endif

ifeq ($(SECURE),1)
	SEC_SRC += $(addprefix iotivity-constrained/security/, oc_acl.c oc_cred.c oc_doxm.c oc_pstat.c \
	                                oc_tls.c oc_svr.c oc_store.c oc_pki.c oc_certs.c oc_sp.c oc_keypair.c oc_csr.c oc_roles.c)
	SRC += $(SEC_SRC)
	SRC_COMMON += ${DTLS}
ifneq ($(DYNAMIC),1)
	SRC_COMMON +=iotivity-constrained/deps/mbedtls/library/memory_buffer_alloc.c
endif
	CFLAGS += ${DTLSFLAGS} -DOC_SECURITY
endif

ifeq ($(IPV4),1)
	CFLAGS += -DOC_IPV4
endif

ifeq ($(COLLECTIONS),1)
	CFLAGS += -DOC_COLLECTIONS
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

LOCAL_C_SRCS += $(SRC_COMMON)
LOCAL_C_SRCS += $(SRC) 


include $(ARDMK_DIR)/Arduino.mk
	
build-$(BOARD_TAG)/libarduino-adapter.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS) 