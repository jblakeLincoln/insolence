#include "android_jni.h"

#ifdef INSOLENCE_ANDROID

extern "C" {

JNIEXPORT void JNICALL
Java_com_insolence_game_InsolenceActivity_nativeSetAssetManager(JNIEnv *env,
		jobject obj, jobject assetManager)
{
	AndroidUtils::Get()->asset_manager =
		AAssetManager_fromJava(env, assetManager);
}

}

#endif
