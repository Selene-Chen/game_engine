#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Shader.h"

// TODO: should be remove
typedef unsigned int GLenum;
namespace Hazel
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        OpenGLShader(const std::string& path);
        ~OpenGLShader();

    public:
        virtual const std::string& GetName() const override { return m_Name; }

    public:
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void SetInt(const std::string& name, const int value) const override;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) const override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) const override;

    public:
        void UploadUniformInt(const std::string& name, const int value) const;
        void UploadUniformFloat(const std::string& name, const float value) const;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        uint32_t m_RendererID;
        std::string m_Name;

    private:
        std::string ReadFile(const std::string& path);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    };
}  // namespace Hazel