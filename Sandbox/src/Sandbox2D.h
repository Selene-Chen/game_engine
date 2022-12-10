#pragma once
#include <Hazel.h>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

class Sandbox2D : public Hazel::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Hazel::Timestep timestep) override;
    virtual void OnEvent(Hazel::Event& event) override;

    virtual void OnImGuiRender() override;

private:
    // CameraController
    Hazel::OrthographicCameraController m_CameraController;

    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Shader> m_FlatColorShader;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};
