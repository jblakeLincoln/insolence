solution "insolence"
	premake.gcc.cc = 'clang'
	premake.gcc.cxx = 'clang++'

	objdir "obj"
	targetdir ( "bin" )
	configurations { "Debug", "Release" }

	configuration "WINDOWS"
		defines "WINDOWS"

	configuration "LINUX"
		defines "LINUX"

	dofile("./src/insolence.lua")
	dofile("./samples/samples.lua")
