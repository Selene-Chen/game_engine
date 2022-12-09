#include "hzpch.h"
#include "Texture.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexturer.h"

namespace Hazel
{

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

} // namespace Hazel