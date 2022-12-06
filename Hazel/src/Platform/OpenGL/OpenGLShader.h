#pragma once
#include "Hazel/Renderer/Shader.h"
namespace Hazel
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
    private:
        uint32_t m_RendererID;
    };
} // namespace Hazel