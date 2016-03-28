solution "insolence"
	objdir "obj"
	targetdir ( "bin" )
	configurations { "Debug", "Release" }

	configuration "WINDOWS"
		defines "WINDOWS"

	configuration "LINUX"
		defines "LINUX"

	dofile("./src/insolence.lua")
	--dofile("./samples/samples.lua")
