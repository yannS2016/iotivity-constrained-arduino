include ../../setup.mk 

ARDUINO_TIME_SRC  = $(wildcard $(ARDUINO_DIR)/libraries/Time/*.cpp)

LOCAL_CPP_SRCS += $(ARDUINO_TIME_SRC) 

include $(ARDMK_DIR)/Arduino.mk 

build-$(BOARD_TAG)/libarduino-time.a: $(LOCAL_OBJS)
	$(AR) rcs $@ $(LOCAL_OBJS)
	
