###iotivity contrained sources
include ../../setup.mk 

SRC=$(wildcard ./*.cpp) 

LOCAL_CPP_SRCS += $(SRC) 
CORE_HEADERS      +=-I$(ARDUINO_DIR)/libraries/SdFat/src/FatLib

CXXFLAGS += $(CORE_HEADERS)
include $(ARDMK_DIR)/Arduino.mk 

build-$(BOARD_TAG)/libarduino-sdfat.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS)
	

