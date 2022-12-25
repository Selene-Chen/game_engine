
#include "hazel/renderer/renderer2d.h"

#include <array>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdint.h>
#include <vcruntime_string.h>
#include <wingdi.h>

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
        float TexIndex;
        float TilingFactor;
    };
    // * 正方形批渲染数据
    struct Render2DDate
    {
        static const uint32_t MaxQuads           = 10000;
        static const uint32_t MaxVertices        = MaxQuads * 4;
        static const uint32_t MaxIndices         = MaxQuads * 6;
        static constexpr int32_t MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Texture2D> WhiteTexture;
        int32_t QuadIndexCount = 0;  // * 顶点索引
        Ref<Shader> TextureShader;

        QuadVertex* QuadVertexBufferBase = nullptr;  // * 顶点数据基址
        QuadVertex* QuadVertexBufferPtr  = nullptr;  // * 顶点数据寻址

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        int32_t TextureSlotIndex = 1;  // 0 = white texture

        std::array<glm::vec4, 4> QuadVertexPositions{};

        Statistics Stats;
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
            {ShaderDataType::Float3,     "a_Position"},
            {ShaderDataType::Float4,        "a_Color"},
            {ShaderDataType::Float2,     "a_TexCoord"},
            { ShaderDataType::Float,     "a_TexIndex"},
            { ShaderDataType::Float, "a_TilingFactor"}
        });
        s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);

        s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];

        // * 创建顶点索引对象结构
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

        // * 创建纹理结构
        std::array<int32_t, Render2DDate::MaxTextureSlots> samplers{};
        for (int32_t i = 0; i < Render2DDate::MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }
        // * shader
        s_data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_data.TextureShader->Bind();
        s_data.TextureShader->SetIntArray("u_Textures", samplers.data(), Render2DDate::MaxTextureSlots);

        //* solt[0] = 空白纹理
        uint32_t white_texture_data = 0xffffffff;
        s_data.WhiteTexture         = Texture2D::Create(1, 1);
        s_data.WhiteTexture->SetData(&white_texture_data, sizeof(uint32_t));
        s_data.TextureSlots[0] = s_data.WhiteTexture;

        s_data.QuadVertexPositions[0] = {-0.5F, -0.5F, 0.0F, 1.0F};
        s_data.QuadVertexPositions[1] = {0.5F, -0.5F, 0.0F, 1.0F};
        s_data.QuadVertexPositions[2] = {0.5F, 0.5F, 0.0F, 1.0F};
        s_data.QuadVertexPositions[3] = {-0.5F, 0.5F, 0.0F, 1.0F};
    }

    void Renderer2D::Shutdown() { HZ_PROFILE_FUNCTION(); }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        HZ_PROFILE_FUNCTION();
        // * 视图变换
        s_data.TextureShader->Bind();
        s_data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        // * 初始化顶点和顶点索引
        s_data.QuadIndexCount      = 0;
        s_data.TextureSlotIndex    = 1;
        s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
    }

    void Renderer2D::EndScene()
    {
        HZ_PROFILE_FUNCTION();
        Flush();
    }

    void Renderer2D::Flush()
    {
        // * 顶点、顶点索引、纹理绑定
        uint32_t data_size = reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferPtr) -
                             reinterpret_cast<uint8_t*>(s_data.QuadVertexBufferBase);
        s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, data_size);
        for (int32_t i = 0; i < s_data.TextureSlotIndex; i++)
        {
            s_data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
        s_data.Stats.DrawCall++;
    }
    void Renderer2D::FlushAndReset()
    {
        EndScene();
        // reset
        s_data.QuadIndexCount      = 0;
        s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;
        s_data.TextureSlotIndex    = 1;
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0F}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION();

        if (s_data.QuadIndexCount >= Render2DDate::MaxQuads)
        {
            FlushAndReset();
        }

        constexpr size_t quad_vertex_count   = 4;
        const float texture_index            = 0.0F;  // White Texture
        constexpr glm::vec2 texture_coords[] = {
            {0.0F, 0.0F},
            {1.0F, 0.0F},
            {1.0F, 1.0F},
            {0.0F, 1.0F}
        };
        const float tiling_factor = 1.0F;

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0F), position) * glm::scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});

        for (size_t i = 0; i < quad_vertex_count; i++)
        {
            s_data.QuadVertexBufferPtr->Position     = transform * s_data.QuadVertexPositions[i];
            s_data.QuadVertexBufferPtr->Color        = color;
            s_data.QuadVertexBufferPtr->TexCoord     = texture_coords[i];
            s_data.QuadVertexBufferPtr->TexIndex     = texture_index;
            s_data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
            s_data.QuadVertexBufferPtr++;
        }

        // * 索引数量：应该是固定6个
        s_data.QuadIndexCount += 6;
        s_data.Stats.QuadCount++;
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                              float tilling_factor, const glm::vec4& tint_color)
    {
        DrawQuad({position.x, position.y, 0.0F}, size, texture, tilling_factor, tint_color);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                              float tiling_factor, const glm::vec4& tint_color)

    {
        HZ_PROFILE_FUNCTION();

        if (s_data.QuadIndexCount >= Render2DDate::MaxQuads)
        {
            FlushAndReset();
        }
        constexpr size_t quad_vertex_count = 4;

        constexpr glm::vec2 texture_coords[] = {
            {0.0F, 0.0F},
            {1.0F, 0.0F},
            {1.0F, 1.0F},
            {0.0F, 1.0F}
        };
        // * 处理纹理
        float texture_index = 0.0F;  // White Texture
        // *纹理已存在于纹理数组
        for (int32_t i = 1; i < s_data.TextureSlotIndex; i++)
        {
            if (s_data.TextureSlots[i] == texture)
            {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        // * 纹理不存在于纹理数组
        if (texture_index == 0.0F)
        {
            // * 纹理槽超过最大
            if (s_data.TextureSlotIndex >= Render2DDate::MaxTextureSlots)
            {
                FlushAndReset();
            }
            texture_index                                = static_cast<float>(s_data.TextureSlotIndex);
            s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
            s_data.TextureSlotIndex++;
        }
        // * 处理变换
        glm::mat4 transform =
            glm::translate(glm::mat4(1.0F), position) * glm::scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});
        for (size_t i = 0; i < quad_vertex_count; i++)
        {
            s_data.QuadVertexBufferPtr->Position     = transform * s_data.QuadVertexPositions[i];
            s_data.QuadVertexBufferPtr->Color        = tint_color;
            s_data.QuadVertexBufferPtr->TexCoord     = texture_coords[i];
            s_data.QuadVertexBufferPtr->TexIndex     = texture_index;
            s_data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
            s_data.QuadVertexBufferPtr++;
        }
        s_data.QuadIndexCount += 6;
        s_data.Stats.QuadCount++;
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
        if (s_data.QuadIndexCount >= Render2DDate::MaxQuads)
        {
            FlushAndReset();
        }

        constexpr size_t quad_vertex_count   = 4;
        const float tiling_factor            = 1.0F;
        const float texture_index            = 0.0F;  // White Texture
        constexpr glm::vec2 texture_coords[] = {
            {0.0F, 0.0F},
            {1.0F, 0.0F},
            {1.0F, 1.0F},
            {0.0F, 1.0F}
        };

        glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) *
                              glm::rotate(glm::mat4(1.0F), glm::radians(rotation), {0.0F, 0.0F, 1.0F}) *
                              glm::scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});

        for (size_t i = 0; i < quad_vertex_count; i++)
        {
            s_data.QuadVertexBufferPtr->Position     = transform * s_data.QuadVertexPositions[i];
            s_data.QuadVertexBufferPtr->Color        = color;
            s_data.QuadVertexBufferPtr->TexCoord     = texture_coords[i];
            s_data.QuadVertexBufferPtr->TexIndex     = texture_index;
            s_data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
            s_data.QuadVertexBufferPtr++;
        }
        s_data.QuadIndexCount += 6;
        s_data.Stats.QuadCount++;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0F}, size, rotation, texture, tiling_factor, tint_color);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
    {
        HZ_PROFILE_FUNCTION();
        if (s_data.QuadIndexCount >= Render2DDate::MaxQuads)
        {
            FlushAndReset();
        }
        constexpr size_t quad_vertex_count = 4;

        constexpr glm::vec2 texture_coords[] = {
            {0.0F, 0.0F},
            {1.0F, 0.0F},
            {1.0F, 1.0F},
            {0.0F, 1.0F}
        };

        // * 处理纹理
        float texture_index = 0.0F;
        // *纹理已存在于纹理数组
        for (int32_t i = 1; i < s_data.TextureSlotIndex; i++)
        {
            if (s_data.TextureSlots[i] == texture)
            {
                texture_index = static_cast<float>(i);
                break;
            }
        }
        // * 纹理不存在于纹理数组
        if (texture_index == 0.0F)
        {
            // * 纹理槽超过最大
            if (s_data.TextureSlotIndex >= Render2DDate::MaxTextureSlots)
            {
                FlushAndReset();
            }
            texture_index                                = static_cast<float>(s_data.TextureSlotIndex);
            s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
            s_data.TextureSlotIndex++;
        }
        glm::mat4 transform = glm::translate(glm::mat4(1.0F), position) *
                              glm::rotate(glm::mat4(1.0F), glm::radians(rotation), {0.0F, 0.0F, 1.0F}) *
                              glm::scale(glm::mat4(1.0F), {size.x, size.y, 1.0F});

        for (size_t i = 0; i < quad_vertex_count; i++)
        {
            s_data.QuadVertexBufferPtr->Position     = transform * s_data.QuadVertexPositions[i];
            s_data.QuadVertexBufferPtr->Color        = tint_color;
            s_data.QuadVertexBufferPtr->TexCoord     = texture_coords[i];
            s_data.QuadVertexBufferPtr->TexIndex     = texture_index;
            s_data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
            s_data.QuadVertexBufferPtr++;
        }
        s_data.QuadIndexCount += 6;
        s_data.Stats.QuadCount++;
    }
    void Renderer2D::ResetStats() { memset(&s_data.Stats, 0, sizeof(Statistics)); }
    Statistics Renderer2D::GetStats() { return s_data.Stats; }
}  // namespace Hazel