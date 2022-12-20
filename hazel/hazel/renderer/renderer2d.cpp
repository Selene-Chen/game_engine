
#include "hazel/renderer/renderer2d.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdint.h>
#include <vadefs.h>

#include "hazel/renderer/render_command.h"
#include "hazel/renderer/shader.h"
#include "hazel/renderer/vertex_array.h"

#include "hzpch.h"

namespace Hazel
{
    // * 正方形顶点数据
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
    };
    // * 正方形批渲染数据
    struct Render2DDate
    {
        const uint32_t MaxQuads    = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices  = MaxQuads * 6;
        uint32_t QuadIndexCount    = 0;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        QuadVertex* QuadVertexBufferBase = nullptr;  // * 基址
        QuadVertex* QuadVertexBufferPtr  = nullptr;
    };

    // * 静态变量，不用管理其生命周期
    static Render2DDate s_data;

    void Renderer2D::Init()
    {
        HZ_PROFILE_FUNCTION();

        s_data.QuadVertexArray = VertexArray::Create();

        // * 创建顶点对象结构
        s_data.QuadVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
        s_data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4,    "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);

        s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];

        // * 创建索引对象结构
        auto* square_indices = new uint32_t[s_data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
        {
            square_indices[i + 0] = offset + 0;
            square_indices[i + 1] = offset + 1;
            square_indices[i + 2] = offset + 2;

            square_indices[i + 3] = offset + 2;
            square_indices[i + 4] = offset + 3;
            square_indices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> square_ibo = IndexBuffer::Create(square_indices, s_data.MaxIndices);
        s_data.QuadVertexArray->SetIndexBuffer(square_ibo);
        delete[] square_indices;

        //* 空白纹理
        uint32_t white_texture_data = 0xffffffff;
        s_data.WhiteTexture         = Texture2D::Create(1, 1);
        s_data.WhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));
        //* shader
        s_data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_data.TextureShader->Bind();
        s_data.TextureShader->SetInt("u_Texture", 0);  // 纹理 Solt
    }

    void Renderer2D::Shutdown() { HZ_PROFILE_FUNCTION(); }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();
        // * 视图变换
        s_data.TextureShader->Bind();
        s_data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        // * 初始化
        s_data.QuadIndexCount      = 0;
        s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
    }
    void Renderer2D::Flush() { RenderCommand::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount); }

    void Renderer2D::EndScene()
    {
        HZ_PROFILE_FUNCTION();

        // * 传输vertex
        uint32_t data_size = reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferPtr) -
                             reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferBase);
        s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, data_size);
        Flush();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0F}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        // * 顶点数据构造，按照左下[0.0]，逆时针顺序
        s_data.QuadVertexBufferPtr->Position = position;
        s_data.QuadVertexBufferPtr->Color    = color;
        s_data.QuadVertexBufferPtr->TexCoord = {0.0F, 0.0F};
        s_data.QuadVertexBufferPtr++;
        s_data.QuadVertexBufferPtr->Position = {position.x + size.x, position.y, position.z};
        s_data.QuadVertexBufferPtr->Color    = color;
        s_data.QuadVertexBufferPtr->TexCoord = {1.0F, 0.0F};
        s_data.QuadVertexBufferPtr++;
        s_data.QuadVertexBufferPtr->Position = {position.x + size.x, position.y + size.y, position.z};
        s_data.QuadVertexBufferPtr->Color    = color;
        s_data.QuadVertexBufferPtr->TexCoord = {1.0F, 1.0F};
        s_data.QuadVertexBufferPtr++;
        s_data.QuadVertexBufferPtr->Position = {position.x, position.y + size.y, position.z};
        s_data.QuadVertexBufferPtr->Color    = color;
        s_data.QuadVertexBufferPtr->TexCoord = {0.0F, 1.0F};
        s_data.QuadVertexBufferPtr++;
        // * 索引数量：应该是固定6个
        s_data.QuadIndexCount += 6;

        // //* 白色纹理
        // s_data.WhiteTexture->Bind(0);
        // //* 纹理平铺系数
        // s_data.TextureShader->SetFloat("u_TilingFactor", 1.0F);
        // //* 变换
        // const glm::mat4 transform =
        //     translate(glm::mat4(1.0F), position) * scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        // s_data.TextureShader->SetMat4("u_Transform", transform);

        // s_data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_data.QuadVertexArray);
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                              float tilling_factor, const glm::vec4& tint_color)
    {
        DrawQuad({position.x, position.y, 0.0F}, size, texture, tilling_factor, tint_color);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                              float tilling_factor, const glm::vec4& tint_color)
    {  // HZ_PROFILE_FUNCTION();

        //*纹理
        texture->Bind(0);
        //*纹理颜色
        s_data.TextureShader->SetFloat4("u_Color", tint_color);
        //*纹理平铺系数
        s_data.TextureShader->SetFloat("u_TilingFactor", tilling_factor);
        //*变换
        const glm::mat4 transform =
            translate(glm::mat4(1.0F), position) * scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        s_data.TextureShader->SetMat4("u_Transform", transform);
        //*顶点数组
        s_data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0F}, size, rotation, color);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        s_data.TextureShader->SetFloat4("u_Color", color);
        s_data.TextureShader->SetFloat("u_TilingFactor", 1.0F);
        s_data.WhiteTexture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) *
                              glm::rotate(glm::mat4(1.0F), rotation, {0.0F, 0.0F, 1.0F}) *
                              glm::scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        s_data.TextureShader->SetMat4("u_Transform", transform);

        s_data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D>& texture, float tilling_factor, const glm::vec4& tint_color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0F}, size, rotation, texture, tilling_factor, tint_color);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D>& texture, float tilling_factor, const glm::vec4& tint_color)
    {
        HZ_PROFILE_FUNCTION();

        s_data.TextureShader->SetFloat4("u_Color", tint_color);
        s_data.TextureShader->SetFloat("u_TilingFactor", tilling_factor);
        texture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) *
                              glm::rotate(glm::mat4(1.0F), rotation, {0.0F, 0.0F, 1.0F}) *
                              glm::scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        s_data.TextureShader->SetMat4("u_Transform", transform);

        s_data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data.QuadVertexArray);
    }
}  // namespace Hazel