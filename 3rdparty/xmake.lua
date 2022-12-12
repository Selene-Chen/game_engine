set_group("3rdparty")
set_kind("static")
set_runtimes("MD")

target("glm")
    add_headerfiles("glm/glm/**.hpp")
    add_includedirs("glm",{interface = true })

target("glad")
    add_files("glad/src/*.c")
    add_headerfiles("glad/include/**.h")
    add_includedirs("glad/include",{ public = true })

target("imgui")
    add_defines("_CRT_SECURE_NO_WARNINGS")
    add_files("imgui/*.cpp")
    add_headerfiles("imgui/**.h")
    add_includedirs("imgui",{public = true })

target("stb")
    add_headerfiles("stb/*.h")
    add_includedirs("stb",{public = true })

target("spdlog")
    add_headerfiles("spdlog/include/**.h")
    add_includedirs("spdlog/include",{ public = true })

target("glfw")
    set_kind("static")
    add_includedirs("/glfw/include",{public = true })
    add_headerfiles("/glfw/include/**.h")
    add_files(
            "glfw/src/context.c",
            "glfw/src/init.c",
            "glfw/src/input.c",
            "glfw/src/monitor.c",
            "glfw/src/vulkan.c",
            "glfw/src/window.c")
    if is_os("windows") then
        add_defines("_GLFW_WIN32","_CRT_SECURE_NO_WARNINGS")
        add_syslinks("user32", "shell32", "gdi32")
        set_optimize("fastest")
        set_runtimes("MD")
        add_files(
                "glfw/src/win32_*.c",
                "glfw/src/wgl_context.c",
                "glfw/src/egl_context.c",
                "glfw/src/osmesa_context.c")
    elseif is_os("linux") then
        add_defines("_GLFW_X11")
        add_syslinks("dl", "pthread")
        add_files(
                "glfw/src/x11_*.c",
                "glfw/src/xkb_unicode.c",
                "glfw/src/posix_time.c",
                "glfw/src/posix_thread.c",
                "glfw/src/glx_context.c",
                "glfw/src/egl_context.c",
                "glfw/src/osmesa_context.c",
                "glfw/src/linux_joystick.c")
    elseif is_os("macosx") then
        add_defines("_GLFW_COCOA")
        add_frameworks("Cocoa", "IOKit")
        add_files(
                "glfw/src/cocoa_*.c",
                "glfw/src/posix_thread.c",
                "glfw/src/nsgl_context.c",
                "glfw/src/egl_context.c",
                "glfw/src/osmesa_context.c")
    end
