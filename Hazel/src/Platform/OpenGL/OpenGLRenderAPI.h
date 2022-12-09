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
        virtual void Init() override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
} // namespace Hazel