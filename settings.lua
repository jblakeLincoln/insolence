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

	configuration { "WINDOWS", "not WebGL*" }
		links { "OpenAL32" }
	configuration { "LINUX", "not WebGL*" }
		links {"openal"}
end

configuration { "WINDOWS or LINUX", "not WebGL*" }
	defines { "INSOLENCE_OPENGL_DESKTOP" }

configuration "WebGL*"
	defines { "INSOLENCE_WEBGL" }

configuration { "LINUX or WebGL*" }
	buildoptions "-std=c++11"

configuration { "LINUX" }
	defines {"LINUX"}

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

configuration { "WINDOWS" }
	defines {"WINDOWS"}
	defines {"NOMINMAX"}
	defines { "SDL_MAIN_HANDLED" }

	includedirs {
		"include",
		"include/freetype2"
	}

	libdirs {
		"lib",
		"bin"
	}

configuration { "WINDOWS", "not WebGL*" }
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

configuration { "Release or WebGL-Release" }
	buildoptions "-O3"
