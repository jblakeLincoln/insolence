	language "C++"
	defines {"GLM_FORCE_RADIANS"}
	objdir "obj"
	targetdir "bin"
	configurations { "Debug", "Release" }

	configuration "LINUX"
		defines {"LINUX"}
		buildoptions "-std=c++11"

		includedirs {
			"/usr/include/bullet",
			"/usr/include/freetype2"
		}

		libdirs {
			"/usr/local/include",
		}

		links {
			"GLEW", "GL", "GLU",
			"glfw3",
			"openal", "alut", "vorbisfile",
			"X11", "Xxf86vm", "Xinerama", "Xcursor", "Xrandr", "pthread", "Xi",
			"assimp",
			"IL", "ILU", "ILUT",
			"BulletDynamics", "BulletCollision", "LinearMath",
			"freetype"
		}

	configuration { "WINDOWS" }
		defines {"WINDOWS"}
		defines {"GLFW_DLL"}

		includedirs {
			"../include"
		}

		libdirs {
			"../lib"
		}

		links {
			"opengl32","GLU32", "glew32",
			"glfw3dll",
			"DevIL", "ILU", "ILUT",
			"assimp",
			"BulletDynamics", "BulletCollision", "LinearMath",
			"freetype"
		}

	configuration "Debug"
		defines {"DEBUG"}
		flags {"Symbols"}

	configuration "Release"
		defines {"NDEBUG"}
		flags {"Optimize"}
