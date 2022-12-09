#pragma once
#include "Hazel/Core.h"
#include <string>
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
        static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
        static Ref<Shader> Create(const std::string& path);
    };
} // namespace Hazel
