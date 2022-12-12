#include "hazel/renderer/shader.h"

#include "hazel/renderer/renderer.h"
#include "platform/opengl/opengl_shader.h"

namespace Hazel
{
    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertex_src, fragment_src);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
    Ref<Shader> Shader::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
            case RenderAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supperted!");
                return nullptr;
            case RenderAPI::API::OpenGL: return CreateRef<OpenGLShader>(path);
        }
        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        HZ_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const auto& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        HZ_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const { return m_shaders.find(name) != m_shaders.end(); }

}  // namespace Hazel