#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, // left down
        0.5f,  -0.5f, 0.0f, // right down
        0.5f,  0.5f,  0.0f, // right top
        -0.5f, 0.5f,  0.0f  // left top
    };
    unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};

    // 1.vertexbuffer and indexbuffer
    Hazel::Ref<Hazel::IndexBuffer>  squareIBO;
    Hazel::Ref<Hazel::VertexBuffer> squareVBO;
    squareVBO = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareIBO = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    // 2.layout
    squareVBO->SetLayout({
        {Hazel::ShaderDataType::Float3, "a_Position"},
    });
    // 3.vertexarray
    m_SquareVA = Hazel::VertexArray::Create();
    m_SquareVA->AddVertexBuffer(squareVBO);
    m_SquareVA->SetIndexBuffer(squareIBO);
    //4.shader
    m_FlatColorShader = Hazel::Shader::Create("assets/shader/FlatColor.glsl");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep) {

    // Update Camera
    m_CameraController.OnUpdate(timestep);

    // Render
    Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Hazel::RenderCommand::Clear();
    // BeginScene
    Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
    // shader uniform
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
    // render submit
    Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    // EndScene
    Hazel::Renderer::EndScene();
}

void Sandbox2D::OnEvent(Hazel::Event& event) { m_CameraController.OnEvent(event); }

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}
