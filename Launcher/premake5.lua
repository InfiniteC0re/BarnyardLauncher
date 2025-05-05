project "BYLauncher"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	
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
		"bzip2.lib",
		"minizip.lib",
		"lzma.lib",
		"zlibstatic-ng.lib",
		"zstd_static.lib",
		"bcrypt.lib"
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
		"%{LibDir.curl}",
		"%{LibDir.minizip}"
	}

	includedirs
	{
		"Source",
		"%{IncludeDir.toshi}",
		"%{IncludeDir.sdl2}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.curl}",
		"%{IncludeDir.minizip}"
	}

	defines
	{
		"TOSHI_CONSOLE",
		"SDL_MAIN_HANDLED"
	}
