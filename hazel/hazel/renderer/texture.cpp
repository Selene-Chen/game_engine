#include "hazel/renderer/texture.h"

#include "hazel/renderer/renderer.h"
#include "platform/opengl/opengl_texture.h"

namespace Hazel
{
    Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

}  // namespace Hazel