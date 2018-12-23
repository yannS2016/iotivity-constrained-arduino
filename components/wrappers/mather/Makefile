###iotivity contrained sources
include ../../../setup.mk 

SRC=$(wildcard ./*.cpp) 

LOCAL_CPP_SRCS += $(SRC) 

include $(ARDMK_DIR)/Arduino.mk  

build-$(BOARD_TAG)/libiotivity-mather.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS)


