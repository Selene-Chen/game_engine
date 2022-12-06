#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
    class Shader
    {
    public:
        Shader()          = default;
        virtual ~Shader() = default;

    public:
        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

    public:
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
} // namespace Hazel
