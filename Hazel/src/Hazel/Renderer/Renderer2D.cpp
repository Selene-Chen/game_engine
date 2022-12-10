#include "hzpch.h"

#include "Renderer2D.h"

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
    };
    static Renderer2DStorage* s_Renderer2DStorage;

    void Renderer2D::Init()
    {
        s_Renderer2DStorage = new Renderer2DStorage();
        s_Renderer2DStorage->QuadVertexArray = VertexArray::Create();

        // clang-format off
        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 
             0.5f, -0.5f, 0.0f, 
             0.5f,  0.5f, 0.0f, 
            -0.5f,  0.5f, 0.0f
        };
        // clang-format on

        Ref<VertexBuffer> squareVBO;
        squareVBO = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        squareVBO->SetLayout({
            {ShaderDataType::Float3, "a_Position"}
        });
        s_Renderer2DStorage->QuadVertexArray->AddVertexBuffer(squareVBO);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> squareIBO;
        squareIBO = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_Renderer2DStorage->QuadVertexArray->SetIndexBuffer(squareIBO);

        s_Renderer2DStorage->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
    }

    void Renderer2D::Shutdown() { delete s_Renderer2DStorage; }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        // shader 变换
       s_Renderer2DStorage->FlatColorShader->Bind();
       s_Renderer2DStorage->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
       s_Renderer2DStorage->FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0f));
    }

    void Renderer2D::EndScene() {}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        // shader 颜色
        s_Renderer2DStorage->FlatColorShader->Bind();
        s_Renderer2DStorage->FlatColorShader->SetFloat3("u_Color", color);
        // VertexArray
        s_Renderer2DStorage->QuadVertexArray->Bind();
        // Draw
        RenderCommand::DrawIndexed(s_Renderer2DStorage->QuadVertexArray);
    }

}  // namespace Hazel