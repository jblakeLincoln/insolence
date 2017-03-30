SOLUTION_INSOLENCE_PATH := $(call my-dir)

include $(SOLUTION_INSOLENCE_PATH)/include/insolence/insolence.mk
include $(SOLUTION_INSOLENCE_PATH)/src/main.mk

include $(SOLUTION_INSOLENCE_PATH)/external/assimp.mk
include $(SOLUTION_INSOLENCE_PATH)/external/freetype.mk
include $(SOLUTION_INSOLENCE_PATH)/SDL2/Android.mk
