
#include "Sandbox2D.h"

#include <imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_camera_controller(1280.0F / 720.0F) {}

void Sandbox2D::OnAttach() { m_texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png"); }

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
        Hazel::RenderCommand::SetClearColor({0.1F, 0.1F, 0.1F, 1});
        Hazel::RenderCommand::Clear();
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
    }

    // TODO(Selene): Add these functions - Shader::SetMat4, Shader::SetFloat4
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color",
    // m_SquareColor);
}

void Sandbox2D::OnEvent(Hazel::Event &event) { m_camera_controller.OnEvent(event); }

void Sandbox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    auto stats = Hazel::Renderer2D::GetStats();

    ImGui::Text("Renderer2d stats:");
    ImGui::Text("Draw Calls:%d", stats.DrawCall);
    ImGui::Text("Quads:%d", stats.QuadCount);
    ImGui::Text("Vertexs:%d", stats.GetTotalVertexCount());
    ImGui::Text("Indices:%d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", value_ptr(m_square_color));

    ImGui::End();
}