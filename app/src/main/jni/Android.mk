LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

SRC_PATH := $(LOCAL_PATH)/../../../../src

LOCAL_MODULE := corejni
LOCAL_SRC_FILES := \
    test.cpp

LOCAL_C_INCLUDES := \
    $(SRC_PATH)/util/include \
    $(SRC_PATH)/nativehelper/include

LOCAL_CFLAGS += -fexceptions
LOCAL_CPPFLAGS += -fexceptions

LOCAL_CPPFLAGS += -std=c++11

ifeq ($(NDK_DEBUG),0)
    LOCAL_CFLAGS += -O3
    LOCAL_CPPFLAGS += -O3
    LOCAL_LDFLAGS += -s
else
    LOCAL_CFLAGS += -O0 -g
    LOCAL_CPPFLAGS += -O0 -g
endif

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
