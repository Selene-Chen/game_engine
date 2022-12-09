#include "hzpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel
{
   
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

} // namespace Hazel
