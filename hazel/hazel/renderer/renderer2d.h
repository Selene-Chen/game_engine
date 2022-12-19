
#pragma once

#include "hazel/renderer/orthographic_camera.h"
#include "hazel/renderer/texture.h"

namespace Hazel
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        // * 带纹理
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling_factor = 1.0F,
                             const glm::vec4& tint_color = glm::vec4(1.0F));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling_factor = 1.0F,
                             const glm::vec4& tint_color = glm::vec4(1.0F));
    };

}  // namespace Hazel