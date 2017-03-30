# SAMPLE_NAME inserted by ./build.sh

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := main

LOCAL_SRC_FILES := ../SDL2/src/main/android/SDL_android_main.c ../include/insolence/util/android_jni.cpp ../../../samples/${SAMPLE_NAME}/main.cpp

LOCAL_C_INCLUDES := /usr/include $(LOCAL_PATH)/../include $(LOCAL_PATH)/../SDL2/include $(LOCAL_PATH)/../include/freetype2
LOCAL_CPP_FEATURES := rtti
LOCAL_SHARED_LIBRARIES := SDL2 assimp freetype insolence
LOCAL_LDLIBS := -landroid -lGLESv1_CM -lGLESv3 -llog -latomic
LOCAL_CFLAGS := -DINSOLENCE_ANDROID -DNDEBUG
LOCAL_CPPFLAGS := -std=c++11 -O3
include $(BUILD_SHARED_LIBRARY)
