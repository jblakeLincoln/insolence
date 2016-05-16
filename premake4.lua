solution "insolence"
	objdir "obj"
	targetdir "bin"

	defines{"INSOLENCE_EXPORT"}

	dofile("./src/insolence.lua")

solution "insolence_samples"
	objdir "obj"
	targetdir "bin"

	includedirs{"src"}
	links{"insolence"}

	dofile("./samples/samples.lua")
