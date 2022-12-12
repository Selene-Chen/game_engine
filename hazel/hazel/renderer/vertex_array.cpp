#include "Hazel/Renderer/Renderer.h"
#include "platform/opengl/opengl_vertex_array.h"

namespace Hazel
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}  // namespace Hazel