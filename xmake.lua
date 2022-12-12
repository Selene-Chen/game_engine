-- define project
set_project("game_engine")
set_xmakever("2.7.3")

-- set common flags
set_languages("cxx20")
set_arch("x64")

-- add build rules
set_toolchains("msvc")
add_rules("plugin.vsxmake.autoupdate")
add_rules("mode.debug","mode.releasedbg", "mode.release")

if is_mode("debug") then
    add_defines("HZ_DEBUG")
    set_symbols("debug")
    set_optimize("none")
    set_runtimes("MDd")
    set_warnings("all")
    set_policy("build.optimization.lto", false)
elseif is_mode("releasedbg") then
    add_defines("HZ_DEBUG")
    set_symbols("debug")
    set_optimize("fastest")
    set_runtimes("MD")
    set_warnings("all")
    set_policy("build.optimization.lto", true)
elseif is_mode("release") then
    add_defines("NDEBUG")
    set_symbols("hidden")
    set_strip("all")
    set_optimize("fastest")
    set_runtimes("MD")
    set_warnings("all", "error")
    set_policy("build.optimization.lto", true)
end

includes("3rdparty")
set_group("engine")

target("hazel")
    set_kind("static")
    add_defines("GLFW_INCLUDE_NONE","_CRT_SECURE_NO_WARNINGS")
    add_files("hazel/**.cpp")
    add_headerfiles("hazel/**.h")
    set_pcxxheader("hazel/hzpch.h")
    add_includedirs("hazel",{ public = true })
    add_deps("spdlog")
    add_deps("glfw")
    add_deps("glad")
    add_deps("stb")
    add_deps("glm")
    add_deps("imgui")

target("sandbox")
    set_kind("binary")
    add_files("sandbox/**.cpp")
    add_headerfiles("sandbox/**.h")
    add_deps("hazel")
    after_build(function (target)
        os.cp("sandbox/assets", target:targetdir())
    end)

