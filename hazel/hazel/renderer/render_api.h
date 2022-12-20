#pragma once
#include <glm/glm.hpp>

#include "hazel/renderer/vertex_array.h"

namespace Hazel
{
    class RenderAPI
    {
    public:
        RenderAPI()          = default;
        virtual ~RenderAPI() = default;

        enum class API
        {
            None   = 0,
            OpenGL = 1
        };

        virtual void Init()                                                                  = 0;
        virtual void SetClearColor(const glm::vec4& color)                                   = 0;
        virtual void Clear()                                                                 = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count) = 0;
        virtual void SetViewPort(int x, int y, int width, int height)                        = 0;

        static API GetAPI() { return m_api; }

        static API m_api;
    };
}  // namespace Hazel