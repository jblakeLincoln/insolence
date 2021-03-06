#ifndef ANDROID_JNI_H
#define ANDROID_JNI_H

#ifdef INSOLENCE_ANDROID

#include "android_utils.h"

#include <SDL2/SDL.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdio>
#include <cerrno>

extern "C" JNIEXPORT void JNICALL
Java_com_insolence_game_InsolenceActivity_nativeSetAssetManager(JNIEnv *env,
		jobject obj, jobject assetManager);

#endif
#endif
