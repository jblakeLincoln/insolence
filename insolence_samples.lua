solution "insolence_samples"
	language "C++"
	targetdir "bin"

	includedirs {
		"src"
	}

	dofile("./samples/blank_project/blank_project.lua")
	dofile("./samples/textured_sprite/textured_sprite.lua")

	configuration { "WINDOWS or LINUX",  "not WebGL*" }
		defines { "INSOLENCE_OPENGL_DESKTOP" }
