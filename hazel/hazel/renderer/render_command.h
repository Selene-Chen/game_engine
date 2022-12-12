#pragma once
#include "hazel/renderer/render_api.h"

namespace Hazel
{
    class RenderCommand
    {
    public:
        RenderCommand() = default;
        ~RenderCommand() = default;

        static void Init() { m_renderer_api->Init(); }

        static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            m_renderer_api->SetViewPort(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
                                        static_cast<int>(height));
        }

        static void SetClearColor(const glm::vec4& color) { m_renderer_api->SetClearColor(color); }
        static void Clear() { m_renderer_api->Clear(); }
        static void DrawIndexed(const Ref<VertexArray>& vertex_array) { m_renderer_api->DrawIndexed(vertex_array); }

    private:
        static Scope<RenderAPI> m_renderer_api;
    };

}  // namespace Hazel