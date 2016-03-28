project "insolence_samples"
	kind "ConsoleApp"

	files { "**.h", "**.cpp" }
	dofile("../settings.lua")

	configuration "LINUX"
		includedirs {
			"../src"
		}

		links { "insolence" }

