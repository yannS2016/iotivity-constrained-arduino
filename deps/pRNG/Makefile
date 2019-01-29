###iotivity contrained sources
include ../../setup.mk 

SRC=$(wildcard ./*.cpp) 

LOCAL_CPP_SRCS += $(SRC) 

include $(ARDMK_DIR)/Arduino.mk 

build-$(BOARD_TAG)/libarduino-prng.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS)
	

