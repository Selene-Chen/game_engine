#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Hazel
{
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

    public:
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void SetMat4(const std::string& name,const glm::mat4& value) const = 0;
        virtual void SetFloat3(const std::string& name,const glm::vec3& value) const = 0;
        virtual void SetFloat4(const std::string& name,const glm::vec4& value) const = 0;

    public:
        virtual const std::string& GetName() const = 0;

    public:
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc,
                                  const std::string& fragmentSrc);
        static Ref<Shader> Create(const std::string& path);
    };
    class ShaderLibrary
    {
    public:
        ShaderLibrary() = default;
        virtual ~ShaderLibrary() = default;

    public:
        void Add(const std::string& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}  // namespace Hazel
