project "Glfw"
    kind "staticLib"
    language "C"
	targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/int/" .. outputdir .. "/%{prj.name}")
    files
    {
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/internal.h",
        "glfw/src/mappings.h",
        "glfw/src/glfw_config.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
    }
    filter "system:windows"
        systemversion "latest"
        staticruntime "on"
        files
        {
            "glfw/src/win32_platform.h",
            "glfw/src/win32_joystick.h",
            "glfw/src/wgl_context.h",
            "glfw/src/egl_context.h",
            "glfw/src/osmesa_context.h",
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }
        defines
		{ 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}
    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "on"

project "Glad"
    kind "staticLib"
    language "C"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/int/" .. outputdir .. "/%{prj.name}")
    files
    {
        "glad/include/glad/glad.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/glad.c",
    }
    includedirs
    {
        "glad/include"
    }
    filter "system:windows"
        systemversion "latest"
        staticruntime "on"
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "on"

project "Imgui"
    kind "staticLib"
    language "C++"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("build/int/" .. outputdir .. "/%{prj.name}")
        files
        {
            "imgui/imconfig.h",
            "imgui/imgui.h",
            "imgui/imgui_internal.h",
            "imgui/imstb_rectpack.h",
            "imgui/imstb_textedit.h",
            "imgui/imstb_truetype.h",
            "imgui/imgui.cpp",
            "imgui/imgui_draw.cpp",
            "imgui/imgui_widgets.cpp",
            "imgui/imgui_tables.cpp",
            "imgui/imgui_demo.cpp",
        }
        filter "system:windows"
            systemversion "latest"
            staticruntime "on"
        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
        filter { "system:windows", "configurations:Release" }
            runtime "Release"
            optimize "on"

project "glm"
    kind "staticLib"
    language "C++"
    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/int/" .. outputdir .. "/%{prj.name}")
    files
    {
        "glm/glm/**"
    }
    includedirs 
    {
        "glm"
    }
    filter "system:windows"
        systemversion "latest"
        staticruntime "on"
        defines
        { 
            "_GLM_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "on"