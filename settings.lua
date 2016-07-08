local include_bullet = false
local include_openal = true

if os.get() == "windows" then
	debugdir "bin"
end

configurations { "Debug", "Release", "WebGL-Debug", "WebGL-Release" }

defines {"GLM_FORCE_RADIANS"}

if include_bullet == true then
	defines{"INSOLENCE_LINKS_BULLET"}

	configuration "not WebGL*"
	links {"BulletDynamics", "BulletCollision", "LinearMath"}
end

if include_openal == true then
	defines{"INSOLENCE_LINKS_OPENAL"}

	configuration "not WebGL*"
		links {"openal"}
end

configuration { "LINUX" }
	defines {"LINUX"}
	buildoptions "-std=c++11"

	includedirs {
		"include",
	}

	libdirs {
		"/usr/local/include",
	}

configuration { "LINUX", "not WebGL*" }
	includedirs {
		"/usr/include",
		"/usr/include/bullet",
		"/usr/include/freetype2"
	}

	links {
		"GLEW", "GL", "GLU",
		"SDL2",
		"X11", "Xxf86vm", "Xinerama", "Xcursor", "Xrandr", "pthread", "Xi",
		"assimp",
		"freetype"
	}

configuration { "WINDOWS", "not WebGL*" }
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

configuration "WebGL*"
	targetextension ".bc"

configuration "Debug or WebGL-Debug"
	defines {"DEBUG"}
	flags {"Symbols"}

configuration "Release or WebGL-Debug"
	defines {"NDEBUG"}
	flags {"Optimize"}

configuration { "Release", "WebGL-Release" }
	buildoptions "-O3"
