#pragma once
#include "hazel/renderer/render_api.h"

namespace Hazel
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        OpenGLRenderAPI() = default;
        ~OpenGLRenderAPI() override = default;

        void Init() override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(const Ref<VertexArray>& vertex_array) override;
        void SetViewPort(int x, int y, int width, int height) override;
    };
}  // namespace Hazel