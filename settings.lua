local include_bullet = false
local include_openal = false

if os.get() == "windows" then
	debugdir "bin"
end

configurations { "Debug", "Release" }

defines {"GLM_FORCE_RADIANS"}

if include_bullet == true then
	defines{"INSOLENCE_LINKS_BULLET"}
	links {"BulletDynamics", "BulletCollision", "LinearMath"}
end

if include_openal == true then
	defines{"INSOLENCE_LINKS_OPENAL"}
	links {"openal", "alut", "vorbisfile"}
end

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
		"SDL2",
		"X11", "Xxf86vm", "Xinerama", "Xcursor", "Xrandr", "pthread", "Xi",
		"assimp",
		"IL", "ILU", "ILUT",
		"freetype"
	}

configuration { "WINDOWS" }
	defines {"WINDOWS"}
	defines {"NOMINMAX"}

	includedirs {
		"include",
		"include/freetype2"
	}

	libdirs {
		"lib",
		"bin"
	}

	links {
		"opengl32","GLU32", "glew32",
		"SDL2",
		"DevIL", "ILU", "ILUT",
		"assimp",
		"freetype"
	}

configuration "Debug"
	defines {"DEBUG"}
	flags {"Symbols"}

configuration "Release"
	defines {"NDEBUG"}
	flags {"Optimize"}
