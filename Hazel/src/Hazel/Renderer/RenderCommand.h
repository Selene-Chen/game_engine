#pragma once
#include "RenderAPI.h"
namespace Hazel
{
    class RenderCommand
    {

    public:
        RenderCommand()  = default;
        ~RenderCommand() = default;

    public:
        inline static void Init() { s_RendererAPI->Init(); }
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
        inline static void Clear() { s_RendererAPI->Clear(); }
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

    private:
        static RenderAPI* s_RendererAPI;
    };

} // namespace Hazel
