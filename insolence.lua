solution "insolence"
	language "C++"
	targetdir "bin"
	objdir "obj"

	configuration { "WINDOWS" }
		defines { "INSOLENCE_EXPORT" }

	dofile("./src/insolence.lua")
