LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := insolence

LOCAL_SRC_FILES := stu.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/../../include $(LOCAL_PATH)/../../include/freetype2
LOCAL_CPP_FEATURES := rtti
LOCAL_SHARED_LIBRARIES := SDL2 assimp freetype
LOCAL_LDLIBS := -landroid -lGLESv1_CM -lGLESv3 -lEGL -latomic
LOCAL_CFLAGS := -DINSOLENCE_ANDROID -DNDEBUG
LOCAL_CPPFLAGS := -std=c++11 -O3
include $(BUILD_SHARED_LIBRARY)
