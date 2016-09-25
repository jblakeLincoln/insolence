include $(CLEAR_VARS)

LOCAL_PATH := $(call my-dir)
LOCAL_MODULE := assimp
LOCAL_SRC_FILES := libassimp.so
include $(PREBUILT_SHARED_LIBRARY)
