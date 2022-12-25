
#include "Sandbox2D.h"

#include <imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_camera_controller(1280.0F / 720.0F) {}

void Sandbox2D::OnAttach()
{
    m_texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
    Hazel::FrameBufferSpec fb_spec;
    fb_spec.Width  = 1280;
    fb_spec.Height = 720;
    m_framebuffer  = Hazel::FrameBuffer::Create(fb_spec);
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(const Hazel::Timestep timestep)
{
    HZ_PROFILE_FUNCTION();
    {
        HZ_PROFILE_SCOPE("controller::Onupdate");
        //   Update Camera
        m_camera_controller.OnUpdate(timestep);
    }
    {
        HZ_PROFILE_SCOPE("render pre");
        //   Render
        m_framebuffer->Bind();

        Hazel::RendererCommand::SetClearColor({0.1F, 0.1F, 0.1F, 1});
        Hazel::RendererCommand::Clear();
    }
    {
        HZ_PROFILE_SCOPE("render draw");

        Hazel::Renderer2D::ResetStats();
        // * BeginScene
        Hazel::Renderer2D::BeginScene(m_camera_controller.GetCamera());

        Hazel::Renderer2D::DrawQuad({0.5F, -0.5F}, {0.5F, 0.75F}, {0.2F, 0.3F, 0.8F, 1.0F});
        Hazel::Renderer2D::DrawQuad({-1.0F, 0.0F}, {0.8F, 0.8F}, {0.8F, 0.2F, 0.3F, 1.0F});

        Hazel::Renderer2D::DrawQuad({0.0F, 0.0F, -0.1F}, {10.0F, 10.0F}, m_texture, 10.F);

        Hazel::Renderer2D::DrawRotatedQuad({1.0F, 0.0F}, {0.8F, 0.8F}, -45.0F, {0.8F, 0.2F, 0.3F, 1.0F});
        Hazel::Renderer2D::DrawRotatedQuad({-2.0F, 0.0F, 0.0F}, {1.0F, 1.0F}, -45.0F, m_texture, 20.0F);

        // * EndScene
        Hazel::Renderer2D::EndScene();

        Hazel::Renderer2D::BeginScene(m_camera_controller.GetCamera());
        // for (float y = -5.0F; y < 5.0F; y += 0.5F)
        // {
        //     for (float x = -5.0F; x < 5.0F; x += 0.5F)
        //     {
        //         glm::vec4 color = {(x + 5.0F) / 10.0F, 0.45F, (y + 5.0F) / 10.0F, 0.75F};
        //         Hazel::Renderer2D::DrawQuad({x, y}, {0.45F, 0.45F}, color);
        //     }
        // }
        Hazel::Renderer2D::EndScene();
        m_framebuffer->UnBind();
    }

    // TODO(Selene): Add these functions - Shader::SetMat4, Shader::SetFloat4
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color",
    // m_SquareColor);
}

void Sandbox2D::OnEvent(Hazel::Event& event) { m_camera_controller.OnEvent(event); }

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    static bool docking_enabled = true;
    if (docking_enabled)
    {
        static bool docker_space_open = true;
        static bool opt_fullscreen    = true;
        static bool opt_padding       = false;

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if ((dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0)
        {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
        }
        ImGui::Begin("DockSpace Demo", &docker_space_open, window_flags);
        if (!opt_padding)
        {
            ImGui::PopStyleVar();
        }

        if (opt_fullscreen)
        {
            ImGui::PopStyleVar(2);
        }

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0F, 0.0F), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    Hazel::Application::GetInstance().Close();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();

        ImGui::Begin("Settings");
        auto stats = Hazel::Renderer2D::GetStats();
        ImGui::Text("Renderer2d stats:");
        ImGui::Text("Draw Calls:%d", stats.DrawCall);
        ImGui::Text("Quads:%d", stats.QuadCount);
        ImGui::Text("Vertexs:%d", stats.GetTotalVertexCount());
        ImGui::Text("Indices:%d", stats.GetTotalIndexCount());

        ImGui::ColorEdit4("Square Color", value_ptr(m_square_color));
        uint32_t id = m_texture->GetRendererID();

        uint32_t texture_id = m_framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(texture_id), ImVec2{1280, 720});

        ImGui::End();
    }
    else
    {
        ImGui::Begin("Settings");
        auto stats = Hazel::Renderer2D::GetStats();
        ImGui::Text("Renderer2d stats:");
        ImGui::Text("Draw Calls:%d", stats.DrawCall);
        ImGui::Text("Quads:%d", stats.QuadCount);
        ImGui::Text("Vertexs:%d", stats.GetTotalVertexCount());
        ImGui::Text("Indices:%d", stats.GetTotalIndexCount());

        ImGui::ColorEdit4("Square Color", value_ptr(m_square_color));
        uint32_t id = m_texture->GetRendererID();
        ImGui::Image(reinterpret_cast<void*>(id), ImVec2{1280, 720});

        ImGui::End();
    }
}
