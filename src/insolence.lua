project "insolence"
	kind "SharedLib"
	files { "**.h", "**.cpp" }

	dofile("../settings.lua")

	configuration "WebGL*"
		targetextension ".bc"
