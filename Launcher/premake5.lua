project "BYLauncher"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	
	-- Set debug dir to the Game folder
	debugdir ("%{wks.location}/../Game")
	
	pchheader "pch.h"
	pchsource "Source/pch.cpp"
	
	links
	{
		"Toshi",
		"ImGui",
		"SDL2.lib",
		"opengl32.lib",
		"glew32s.lib",
		"libcurl.dll.lib",
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.hpp"
	}
			
	libdirs
	{
		"%{LibDir.sdl2}",
		"%{LibDir.glew}",
		"%{LibDir.curl}"
	}

	includedirs
	{
		"Source",
		"%{IncludeDir.toshi}",
		"%{IncludeDir.sdl2}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.curl}"
	}

	defines
	{
		"TOSHI_CONSOLE",
		"SDL_MAIN_HANDLED"
	}
