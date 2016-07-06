solution "insolence"
	language "C++"
	objdir "obj"
	targetdir "bin"

	defines{"INSOLENCE_EXPORT"}
	defines{"INSOLENCE_OPENGL_DESKTOP"}

	dofile("./src/insolence.lua")

solution "insolence_samples"
	language "C++"
	objdir "obj"
	targetdir "bin"

	defines{"INSOLENCE_OPENGL_DESKTOP"}
	includedirs{"src"}
	links{"insolence"}

	dofile("./samples/samples.lua")
