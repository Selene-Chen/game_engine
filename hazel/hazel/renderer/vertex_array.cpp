#include "hazel/renderer/renderer.h"
#include "platform/opengl/opengl_vertex_array.h"

namespace Hazel
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}  // namespace Hazel