#pragma once
#include "hazel/renderer/framebuffer.h"

#include "hazel/renderer/renderer.h"
#include "platform/opengl/opengl_framebuffer.h"

namespace Hazel
{

    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpec& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}  // namespace Hazel