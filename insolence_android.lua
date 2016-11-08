require "premake/android/androidmk"

solution "Insolence"

	configurations { "Debug", "Release" }
	language "C++"

	location "android/jni"

	ndkabi "armeabi"
	ndkplatform "android-18"
	ndkstl "gnustl_static"
	ndktoolchainversion "clang"

	rtti "On"
	flags { "C++11" }

	filter "configurations:Release"
		optimize "On"
	filter "configurations:Debug"
		optimize "Debug"

	project "insolence"
		kind "SharedLib"
		location "android/jni/include/insolence"

		defines "INSOLENCE_ANDROID"

		files {
			"android/jni/include/insolence/**.cpp",
		}

		includedirs {
			"android/jni/include",
			"/usr/include/freetype2/",
		}

		links {
			"android",
			"GLESv1_CM",
			"GLESv3",
			"EGL",
		}

		amk_sharedlinks {
			"SDL2",
			"assimp",
			"freetype",
		}

		amk_includes {
			"android/jni/external/assimp.mk",
			"android/jni/external/freetype.mk",
		}

	project "main"
		kind "SharedLib"
		location "android/jni/src"
		defines "INSOLENCE_ANDROID"

		files {
			"samples/**.cpp",
			"android/jni/SDL2/src/main/android/SDL_android_main.c",
			"android/jni/include/insolence/util/android_jni.cpp"
		}

		includedirs {
			"android/jni/include",
			"android/jni/SDL2/include",
			"/usr/include/freetype2/"
		}

		links {
			"android",
			"GLESv1_CM",
			"GLESv3",
			"log"
		}

		amk_includes {
			"android/jni/SDL2/Android.mk"
		}

		amk_sharedlinks {
			"SDL2",
			"assimp",
			"freetype",
			"insolence",
		}
