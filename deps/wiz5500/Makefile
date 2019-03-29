include ../../setup.mk 

SRC=$(wildcard ./*.cpp) 
LOCAL_CPP_SRCS += $(SRC)

CXXFLAGS +=-I../../adapter/ipadapter/include -I../../adapter/iotivity-constrained/port \
          -I$(ARDUINO_DIR)/libraries/Ethernet2/src/utility -I../serial

include $(ARDMK_DIR)/Arduino.mk 

build-$(BOARD_TAG)/libarduino-wiz5500.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS)
	

