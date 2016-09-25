#ifndef ANDROID_JNI_H
#define ANDROID_JNI_H

#ifdef INSOLENCE_ANDROID

#include "android_utils.h"

#include <SDL.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdio>
#include <cerrno>

extern "C" JNIEXPORT void JNICALL
Java_com_insolence_game_InsolenceActivity_nativeSetAssetManager(JNIEnv *env,
		jobject obj, jobject assetManager)
{
	AndroidUtils::Get()->asset_manager =
		AAssetManager_fromJava(env, assetManager);
}

#endif
#endif
