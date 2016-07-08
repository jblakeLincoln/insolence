project "insolence_samples"
	kind "ConsoleApp"

	files { "**.h", "**.cpp" }
	dofile("../settings.lua")

	configuration { "LINUX" }
		includedirs {
			"../src"
		}

	configuration { "LINUX", "not WebGL*" }
		links { "insolence" }

