solution "insolence"
	language "C++"
	objdir "obj"
	targetdir "bin"

	defines{"INSOLENCE_EXPORT"}

	dofile("./src/insolence.lua")

solution "insolence_samples"
	language "C++"
	objdir "obj"
	targetdir "bin"

	includedirs{"src"}
	links{"insolence"}

	dofile("./samples/samples.lua")
