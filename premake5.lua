include "OpenBarnyard/Settings.lua"
include "Dependencies.lua"

outputdir = "%{cfg.buildcfg}_%{cfg.platform}_%{cfg.architecture}"

workspace "Barnyard Launcher"
	cppdialect "C++20"
	characterset "ASCII"
	architecture "x86"
	
	startproject "BYLauncher"

	platforms "Windows"
	configurations { "Debug", "Release", "Final" }

	disablewarnings { "4996" }
	
	debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	editandcontinue "Off"
	
	-- Global defines
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"TMEMORY_USE_DLMALLOC",
	--	"TOSHI_PROFILER",
	--	"TOSHI_PROFILER_MEMORY",
	--	"TRACY_ENABLE"
	}

	-- Global Windows parameters
	filter "system:windows"
		systemversion "latest"
		
		vectorextensions "SSE2"
		
		defines
		{
			"TOSHI_SKU_WINDOWS"
		}
		
		defines
		{
			"TRENDERINTERFACE_GL"
		}
		
		defines
		{
			"GLEW_STATIC",
			"GLM_FORCE_LEFT_HANDED"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines "TOSHI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines "TOSHI_RELEASE"
		optimize "On"

	filter "configurations:Final"
		runtime "Release"
		defines "TOSHI_FINAL"
		optimize "On"

-- Include the projects

group "Main"
	include "OpenBarnyard/Toshi"
	include "Launcher"
	
group "ThirdParty"
	include "ThirdParty/ImGui"
