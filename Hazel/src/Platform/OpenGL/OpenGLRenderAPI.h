#pragma once
#include "Hazel/Renderer/RenderAPI.h"

namespace Hazel
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        OpenGLRenderAPI()  = default;
        ~OpenGLRenderAPI() = default;

    public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
} // namespace Hazel