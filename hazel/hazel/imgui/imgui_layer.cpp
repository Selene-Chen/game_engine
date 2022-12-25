
#include "hazel/imgui/imgui_layer.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "hazel/core/application.h"

#include "hzpch.h"

namespace Hazel
{
    ImGuiLayer::ImGuiLayer() : Layer("ImguiLayer") {}
    void ImGuiLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &imgui_io = ImGui::GetIO();
        (void)imgui_io;

        imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
        // Controls
        imgui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
        imgui_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform
                                                                   // Windows
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform
        // windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if ((imgui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            style.WindowRounding              = 0.0F;
            style.Colors[ImGuiCol_WindowBg].w = 1.0F;
        }

        // 获取窗口和设置glsl版本
        Application &app               = Application::GetInstance();
        auto *const window             = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());
        const auto *const glsl_version = "#version 410";

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        imgui_io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values
                                                                          // (optional)
        imgui_io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos
                                                                          // requests (optional, rarely used)
        imgui_io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;  // We can create multi-viewports
                                                                          // on the Platform side (optional)
    }

    void ImGuiLayer::OnDetach()
    {
        HZ_PROFILE_FUNCTION();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender() {}

    void ImGuiLayer::Begin()
    {
        HZ_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        HZ_PROFILE_FUNCTION();

        ImGuiIO &imgui_io = ImGui::GetIO();
        Application &app  = Application::GetInstance();
        imgui_io.DisplaySize =
            ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we
        // save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call
        //  glfwMakeContextCurrent(window) directly)
        if ((imgui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}  // namespace Hazel