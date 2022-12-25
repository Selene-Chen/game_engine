
#pragma once

#include "hazel/renderer/orthographic_camera.h"
#include "hazel/renderer/texture.h"

namespace Hazel
{
    struct Statistics
    {
        uint32_t DrawCall  = 0;
        uint32_t QuadCount = 0;
        [[nodiscard]] uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
        [[nodiscard]] uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
    };
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();  // * 批渲染

        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        // * 带纹理
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                             float tilling_factor = 1.0F, const glm::vec4& tint_color = glm::vec4(1.0F));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                             float tiling_factor = 1.0F, const glm::vec4& tint_color = glm::vec4(1.0F));

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D>& texture, float tiling_factor = 1.0F,
                                    const glm::vec4& tint_color = glm::vec4(1.0F));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D>& texture, float tiling_factor = 1.0F,
                                    const glm::vec4& tint_color = glm::vec4(1.0F));
        static void ResetStats();
        static Statistics GetStats();

    private:
        static void FlushAndReset();
    };

}  // namespace Hazel