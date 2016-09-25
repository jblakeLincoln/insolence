#include "android_utils.h"

#ifdef INSOLENCE_ANDROID

AndroidUtils* AndroidUtils::instance;

FILE* AndroidUtils::android_fopen(const char* fname, const char* mode) {
	AAsset *asset;

	if(mode[0] == 'w')
		return NULL;

	asset = AAssetManager_open(AndroidUtils::asset_manager, fname, 0);

	if(asset == NULL)
		return NULL;

	return funopen(asset, AndroidUtils::Read, AndroidUtils::Write,
			AndroidUtils::Seek, AndroidUtils::Close);
}

int AndroidUtils::Read(void* cookie, char* buf, int size)
{
	return AAsset_read((AAsset*)cookie, buf, size);
}

int AndroidUtils::Write(void* cookie, const char* buf, int size)
{
	return EACCES;
}

fpos_t AndroidUtils::Seek(void* cookie, fpos_t offset, int whence)
{
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

int AndroidUtils::Close(void* cookie)
{
	AAsset_close((AAsset*)cookie);
	return 0;
}

#endif
