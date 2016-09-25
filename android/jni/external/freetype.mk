include $(CLEAR_VARS)

LOCAL_PATH := $(call my-dir)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := libfreetype.so
include $(PREBUILT_SHARED_LIBRARY)
