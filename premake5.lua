workspace "game_engine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Hazel"] = "Hazel/src"
IncludeDir["spdlog"] = "3rdparty/spdlog/include"
IncludeDir["glfw"]= "3rdparty/glfw/include"
IncludeDir["glad"]= "3rdparty/glad/include"
IncludeDir["imgui"]= "3rdparty/imgui"
IncludeDir["glm"]= "3rdparty/glm"

include "3rdparty"

project "Hazel"
	location "Hazel"
	kind "staticLib"
	language "C++"
	staticruntime "on"

	targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/int/" .. outputdir .. "/%{prj.name}")
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	includedirs
	{
		"%{IncludeDir.Hazel}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
	}
	links
	{
		"opengl32.lib",
		"Glfw",
		"Glad",
		"Imgui",
		"glm"
	}
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../build/bin/" .. outputdir .. "/Sandbox/\"")
		}
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	includedirs
	{
		"%{IncludeDir.Hazel}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hazel"
	}
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"