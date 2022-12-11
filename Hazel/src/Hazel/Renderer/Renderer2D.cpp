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
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };
    static Renderer2DStorage* s_Renderer2DStorage;

    void Renderer2D::Init()
    {
        s_Renderer2DStorage = new Renderer2DStorage();
        s_Renderer2DStorage->QuadVertexArray = VertexArray::Create();

        // clang-format off
        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f,0.0f,
             0.5f, -0.5f, 0.0f, 1.0f,0.0f,
             0.5f,  0.5f, 0.0f, 1.0f,1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f,1.0f
        };
        // clang-format on

        Ref<VertexBuffer> squareVBO;
        squareVBO = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        squareVBO->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_Renderer2DStorage->QuadVertexArray->AddVertexBuffer(squareVBO);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> squareIBO;
        squareIBO = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_Renderer2DStorage->QuadVertexArray->SetIndexBuffer(squareIBO);
        // 空白纹理
        uint32_t whiteTextureData = 0xffffffff;
        s_Renderer2DStorage->WhiteTexture = Texture2D::Create(1, 1);
        s_Renderer2DStorage->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        // shader
        s_Renderer2DStorage->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Renderer2DStorage->TextureShader->Bind();
        s_Renderer2DStorage->TextureShader->SetInt("u_Texture", 0);  // 纹理 Solt
    }

    void Renderer2D::Shutdown() { delete s_Renderer2DStorage; }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        // Shader 视图变换
        s_Renderer2DStorage->TextureShader->Bind();
        s_Renderer2DStorage->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
       
        s_Renderer2DStorage->TextureShader->SetMat4("u_Transform", transform);// 模型变换
        s_Renderer2DStorage->TextureShader->SetFloat4("u_Color", color);// 颜色
        s_Renderer2DStorage->WhiteTexture->Bind(); // 纹理

        s_Renderer2DStorage->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Renderer2DStorage->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        s_Renderer2DStorage->TextureShader->SetMat4("u_Transform", transform);//变换
        s_Renderer2DStorage->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));//颜色
        texture->Bind();  //纹理 默认Solt=0

        s_Renderer2DStorage->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Renderer2DStorage->QuadVertexArray);
    }

}  // namespace Hazel