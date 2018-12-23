include ./setup.mk
MATHER_ARCHIVE 		= build-$(BOARD_TAG)/libiotivity-mather.a
SERIAL_ARCHIVE 		= build-$(BOARD_TAG)/libiotivity-serial.a
SERVER_ARCHIVE 		= build-$(BOARD_TAG)/libiotivity-constrained.a
PRNG_ARCHIVE 			= build-$(BOARD_TAG)/libiotivity-prng.a

LOCAL_CPP_SRCS 		+= main/main.cpp
CPPFLAGS 					+= -DOC_SERVER=1

#CPPFLAGS += -DARDUINO_SERIAL=0

SERVER_OBJ     = $(PROJECT_DIR)/components/adapter/$(SERVER_ARCHIVE)
OTHER_OBJS    += $(SERVER_OBJ)

WRAPPERS_LIBS += $(PROJECT_DIR)/components/wrappers/mather/$(MATHER_ARCHIVE)
WRAPPERS_LIBS += $(PROJECT_DIR)/components/wrappers/IotivitySerial/$(SERIAL_ARCHIVE)
WRAPPERS_LIBS += $(PROJECT_DIR)/components/wrappers/pRNG/$(PRNG_ARCHIVE)	

OTHER_LIBS  	+= $(WRAPPERS_LIBS) 

#ifeq ($(ARDUINOSERIAL),1)
#	CFLAGS += -DARDUINO_SERIAL
#  CPPLAGS += -DARDUINO_SERIAL
#endif

include $(ARDMK_DIR)/Arduino.mk

$(SERVER_OBJ): MATHER_ARCHIVE SERIAL_ARCHIVE PRNG_ARCHIVE
	$(MAKE) -C ./components/adapter $(SERVER_ARCHIVE)
	
MATHER_ARCHIVE: 
	$(MAKE) -C ./components/wrappers/mather $(MATHER_ARCHIVE)	
	
SERIAL_ARCHIVE: 
	$(MAKE) -C ./components/wrappers/IotivitySerial $(SERIAL_ARCHIVE)	


PRNG_ARCHIVE: 
	$(MAKE) -C ./components/wrappers/pRNG $(PRNG_ARCHIVE)	
  
  	
clean::
	$(MAKE) -C ./components/adapter clean
	$(MAKE) -C ./components/wrappers/pRNG clean  
	$(MAKE) -C ./components/wrappers/mather clean
	$(MAKE) -C ./components/wrappers/IotivitySerial clean
