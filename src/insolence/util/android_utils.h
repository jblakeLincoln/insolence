#ifndef ANDROID_UTILS_H
#define ANDROID_UTILS_H

#ifdef INSOLENCE_ANDROID

#include <SDL2/SDL.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdio>
#include <cerrno>

struct AndroidUtils {
private:
	static AndroidUtils *instance;

	static int Read(void* cookie, char* buf, int size);
	static int Write(void* cookie, const char* buf, int size);
	static fpos_t Seek(void* cookie, fpos_t offset, int whence);
	static int Close(void* cookie);

public:
	AAssetManager* asset_manager;

	static AndroidUtils* Get() {
		if(AndroidUtils::instance == NULL)
			AndroidUtils::instance = new AndroidUtils();
		return instance;
	}

	FILE* android_fopen(const char* fname, const char* mode);
};

#define fopen(name, mode) AndroidUtils::Get()->android_fopen(name, mode)

#endif
#endif
