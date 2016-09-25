solution "insolence"
	language "C++"
	targetdir "bin"
	objdir "obj"

	configuration { "WINDOWS" }
		defines { "INSOLENCE_EXPORT" }

	dofile("./src/insolence.lua")


solution "insolence_samples"
	language "C++"
	targetdir "bin"
	objdir "obj"

	includedirs {
		"src"
	}

	dofile("./samples/samples.lua")

	configuration { "WINDOWS or LINUX",  "not WebGL*" }
		defines { "INSOLENCE_OPENGL_DESKTOP" }
