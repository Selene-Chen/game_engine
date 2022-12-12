#include "Sandbox2D.h"

#include <imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_camera_controller(1280.0F / 720.0F) {}

void Sandbox2D::OnAttach() { m_texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png"); }

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(const Hazel::Timestep timestep)
{
    // Update Camera
    m_camera_controller.OnUpdate(timestep);

    // Render
    Hazel::RenderCommand::SetClearColor({0.1F, 0.1F, 0.1F, 1});
    Hazel::RenderCommand::Clear();
    // BeginScene
    Hazel::Renderer2D::BeginScene(m_camera_controller.GetCamera());
    Hazel::Renderer2D::DrawQuad({-1.0F, 0.0F}, {0.8F, 0.8F}, {0.8F, 0.2F, 0.3F, 1.0F});
    Hazel::Renderer2D::DrawQuad({0.5F, -0.5F}, {0.5F, 0.75F}, {0.2F, 0.3F, 0.8F, 1.0F});
    Hazel::Renderer2D::DrawQuad({0.0F, 0.0F, -0.1F}, {10.0F, 10.0F}, m_texture);
    // EndScene
    Hazel::Renderer::EndScene();

    // TODO(Selene): Add these functions - Shader::SetMat4, Shader::SetFloat4
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnEvent(Hazel::Event& event) { m_camera_controller.OnEvent(event); }

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", value_ptr(m_square_color));
    ImGui::End();
}