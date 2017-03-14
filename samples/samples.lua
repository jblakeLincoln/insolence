kind "ConsoleApp"
dofile("../insolence_settings.lua")

configuration { "LINUX" }
	includedirs {
		"../src"
	}

	libdirs {
		"../bin"
	}

configuration { "LINUX", "not WebGL*" }
	links { "insolence" }

