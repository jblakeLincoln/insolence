APP_BUILD_SCRIPT := $(call my-dir)/Insolence_Android.mk
PM5_HELP := true

ifeq ($(PM5_CONFIG),debug)
  APP_ABI := armeabi
endif

ifeq ($(PM5_CONFIG),release)
  APP_ABI := armeabi x86
endif

APP_OPTIM := release
APP_PLATFORM := android-18
APP_STL := gnustl_static
NDK_TOOLCHAIN_VERSION := clang
PM5_HELP := false

NDK_DEBUG=0

ifeq ($(PM5_HELP),true)
  $(info )
  $(info Usage:)
  $(info $()  ndk-build PM5_CONFIG=<config>)
  $(info )
  $(info CONFIGURATIONS:)
  $(info $()  debug)
  $(info $()  release)
  $(info )
  $(info For more ndk-build options, see https://developer.android.com/ndk/guides/ndk-build.html)
  $(info )
  $(error Set PM5_CONFIG and try again)
endif
