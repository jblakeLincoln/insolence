configurations { "Debug", "Release", "WebGL-Debug", "WebGL-Release" }

if os.get() == "windows" then
	debugdir "bin"
end

defines {"GLM_FORCE_RADIANS"}
defines{"INSOLENCE_LINKS_OPENAL"}

configuration { "WINDOWS or LINUX", "not WebGL*" }
	defines { "INSOLENCE_OPENGL_DESKTOP" }

configuration "WebGL*"
	defines { "INSOLENCE_WEBGL" }

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
		"openal",
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
		"OpenAL32",
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
