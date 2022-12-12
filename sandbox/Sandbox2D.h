#pragma once
#include <glm/gtc/matrix_transform.hpp>

#include "hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Hazel::Timestep timestep) override;
    void OnEvent(Hazel::Event& event) override;

    void OnImGuiRender() override;

private:
    // CameraController
    Hazel::OrthographicCameraController m_camera_controller;

    Hazel::Ref<Hazel::VertexArray> m_square_va;
    Hazel::Ref<Hazel::Shader> m_flat_color_shader;
    Hazel::Ref<Hazel::Texture2D> m_texture;
    glm::vec4 m_square_color = {0.2F, 0.3F, 0.8F, 1.0F};
};
