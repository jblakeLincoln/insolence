solution "insolence"
	language "C++"
	targetdir "bin"
	objdir "obj"

	dofile("./src/insolence.lua")

	configuration { "WINDOWS or LINUX", "not WebGL*" }
		defines { "INSOLENCE_OPENGL_DESKTOP" }
	configuration "WebGL*"
		defines { "INSOLENCE_WEBGL" }
		defines { "INSOLENCE_OPENGL_ES" }

solution "insolence_samples"
	language "C++"
	targetdir "bin"
	objdir "obj"

	dofile("./samples/samples.lua")

	configuration { "WINDOWS or LINUX",  "not WebGL*" }
		defines { "INSOLENCE_OPENGL_DESKTOP" }
	configuration "WebGL*"
		links { "insolence" }
		defines { "INSOLENCE_WEBGL" }
		defines { "INSOLENCE_OPENGL_ES" }
