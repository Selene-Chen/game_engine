#pragma once
#include <stdint.h>

#include "hazel/renderer/renderer_api.h"

namespace Hazel
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        OpenGLRendererAPI()           = default;
        ~OpenGLRendererAPI() override = default;

        void Init() override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count) override;
        void SetViewPort(int x, int y, int width, int height) override;
    };
}  // namespace Hazel