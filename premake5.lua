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

project "Hazel"
	location "Hazel"
	kind "staticLib"
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
		"%{IncludeDir.glfw}",
	}
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
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
			"HZ_PLATFORM_WINDOWS"
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