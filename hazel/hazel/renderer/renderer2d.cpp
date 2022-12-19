
#include "hazel/renderer/renderer2d.h"

#include <array>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "hazel/renderer/render_command.h"
#include "hazel/renderer/shader.h"
#include "hazel/renderer/vertex_array.h"

#include "hzpch.h"

namespace Hazel
{
    struct Render2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };
    static Ref<Render2DStorage> s_Renderer2dStorage;

    void Renderer2D::Init()
    {
        HZ_PROFILE_FUNCTION();

        s_Renderer2dStorage = CreateRef<Render2DStorage>();

        s_Renderer2dStorage->QuadVertexArray = VertexArray::Create();

        // clang-format off
        std::array square_vertices= {
            -0.5F, -0.5F, 0.0F, 0.0F,0.0F,
            0.5F, -0.5F, 0.0F, 1.0F,0.0F,
            0.5F,  0.5F, 0.0F, 1.0F,1.0F,
            -0.5F,  0.5F, 0.0F, 0.0F,1.0F
        };
        // clang-format on

        Ref<VertexBuffer> square_vbo = VertexBuffer::Create(square_vertices.data(), sizeof(square_vertices));
        square_vbo->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_Renderer2dStorage->QuadVertexArray->AddVertexBuffer(square_vbo);

        std::array<uint32_t, 6> square_indices = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> square_ibo            = IndexBuffer::Create(square_indices.data(), sizeof(square_indices) / sizeof(uint32_t));
        s_Renderer2dStorage->QuadVertexArray->SetIndexBuffer(square_ibo);
        // 空白纹理
        uint32_t white_texture_data       = 0xffffffff;
        s_Renderer2dStorage->WhiteTexture = Texture2D::Create(1, 1);
        s_Renderer2dStorage->WhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));
        // shader
        s_Renderer2dStorage->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Renderer2dStorage->TextureShader->Bind();
        s_Renderer2dStorage->TextureShader->SetInt("u_Texture", 0);  // 纹理 Solt
    }

    void Renderer2D::Shutdown()
    {
        HZ_PROFILE_FUNCTION();
        std::default_delete<Render2DStorage>();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();
        // Shader 视图变换
        s_Renderer2dStorage->TextureShader->Bind();
        s_Renderer2dStorage->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() { HZ_PROFILE_FUNCTION(); }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0F}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        //*颜色
        s_Renderer2dStorage->TextureShader->SetFloat4("u_Color", color);
        //*白色纹理
        s_Renderer2dStorage->WhiteTexture->Bind(0);
        //*纹理平铺系数
        s_Renderer2dStorage->TextureShader->SetFloat("u_TilingFactor", 1.0F);
        //*变换
        const glm::mat4 transform = translate(glm::mat4(1.0F), position) * scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        s_Renderer2dStorage->TextureShader->SetMat4("u_Transform", transform);

        s_Renderer2dStorage->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Renderer2dStorage->QuadVertexArray);
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling_factor,
                              const glm::vec4& tint_color)
    {
        DrawQuad({position.x, position.y, 0.0F}, size, texture, tilling_factor, tint_color);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling_factor,
                              const glm::vec4& tint_color)
    {  // HZ_PROFILE_FUNCTION();

        //*纹理
        texture->Bind(0);
        //*纹理颜色
        s_Renderer2dStorage->TextureShader->SetFloat4("u_Color", tint_color);
        //*纹理平铺系数
        s_Renderer2dStorage->TextureShader->SetFloat("u_TilingFactor", tilling_factor);
        //*变换
        const glm::mat4 transform = translate(glm::mat4(1.0F), position) * scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        s_Renderer2dStorage->TextureShader->SetMat4("u_Transform", transform);
        //*顶点数组
        s_Renderer2dStorage->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Renderer2dStorage->QuadVertexArray);
    }

}  // namespace Hazel