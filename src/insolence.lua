project "insolence"
	kind "SharedLib"
	files { "**.h", "**.cpp" }

	dofile("../insolence_settings.lua")

	configuration "WebGL*"
		targetextension ".bc"
