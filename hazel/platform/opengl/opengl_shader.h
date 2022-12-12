#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "hazel/renderer/shader.h"

namespace Hazel
{
    // TODO(Selene): should be remove
    using GLenum = unsigned int;
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(std::string name, const std::string& vertex_src, const std::string& fragment_src);
        explicit OpenGLShader(const std::string& path);
        ~OpenGLShader() override;

        [[nodiscard]] const std::string& GetName() const override { return m_name; }

        void Bind() const override;
        void UnBind() const override;
        void SetInt(const std::string& name, int value) const override;
        void SetMat4(const std::string& name, const glm::mat4& value) const override;
        void SetFloat3(const std::string& name, const glm::vec3& value) const override;
        void SetFloat4(const std::string& name, const glm::vec4& value) const override;

        void UploadUniformInt(const std::string& name, int value) const;
        void UploadUniformFloat(const std::string& name, float value) const;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        uint32_t m_renderer_id = 0;
        std::string m_name;

        static std::string ReadFile(const std::string& path);
        static std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shader_sources);
    };
}  // namespace Hazel