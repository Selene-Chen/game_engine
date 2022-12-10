#pragma once
#include "Hazel/Renderer/RenderAPI.h"

namespace Hazel
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        OpenGLRenderAPI() = default;
        ~OpenGLRenderAPI() = default;

    public:
        virtual void Init() override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    };
}  // namespace Hazel