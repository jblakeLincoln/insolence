solution "insolence"
	objdir "obj"
	targetdir "bin"
	configurations { "Debug", "Release" }

	defines {"GLM_FORCE_RADIANS"}
	configuration {"LINUX"}
		defines {"LINUX"}

	project "insolence"
		kind "ConsoleApp"
		language "C++"

		defines { "GLFW_DLL" }
		files { "**.h", "**.cpp" }

	configuration "LINUX"
		includedirs {
			"/usr/include/bullet"
		}

		libdirs {
			"/usr/local/include",
		}

		links {
			"glfw3", "GLEW", "GL", "GLU", "openal", "alut", "vorbisfile",
			"X11", "Xxf86vm", "Xinerama", "Xcursor", "Xrandr", "pthread", "Xi",
			"assimp", "IL", "ILU", "ILUT", "BulletDynamics", "BulletCollision", "LinearMath"
		}

	configuration { "WINDOWS" }
			includedirs {
				"../include"
			}

			libdirs {
				"../lib"
			}

			links {
				"opengl32", "glfw3dll", "GLU32", "glew32", "DevIL", "ILU", "ILUT",
				"assimp", "BulletDynamics", "BulletCollision", "LinearMath",
			}

	configuration "Debug"
		defines {"DEBUG"}
		flags {"Symbols"}

	configuration "Release"
		defines {"NDEBUG"}
		flags {"Optimize"}
