#ifndef INSOLENCE_DLL_H
#define INSOLENCE_DLL_H

#ifdef WINDOWS
	#ifdef INSOLENCE_EXPORT
		#define INSOLENCE_API __declspec(dllexport)
	#else
		#define INSOLENCE_API __declspec(dllimport)
	#endif
#else
	#define INSOLENCE_API
#endif

#ifdef INSOLENCE_ANDROID
	#include "util/android_utils.h"
#endif

#endif
