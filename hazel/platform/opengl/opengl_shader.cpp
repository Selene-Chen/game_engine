#include "platform/opengl/opengl_shader.h"

#include <array>
#include <fstream>
#include <glad/glad.h>

#include "hazel/core/core.h"

#include "hzpch.h"

namespace Hazel
{
    static GLenum ShaderTypeFromString(const std::string &type)
    {
        if (type == "vertex")
        {
            return GL_VERTEX_SHADER;
        }
        if (type == "fragment" || type == "pixel")
        {
            return GL_FRAGMENT_SHADER;
        }
        HZ_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }
    OpenGLShader::OpenGLShader(std::string name, const std::string &vertex_src, const std::string &fragment_src)
        : m_name(std::move(name))
    {
        HZ_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertex_src;
        sources[GL_FRAGMENT_SHADER] = fragment_src;
        Compile(sources);
    }

    OpenGLShader::OpenGLShader(const std::string &path)
    {
        HZ_PROFILE_FUNCTION();
        const auto source = ReadFile(path);
        const auto shader_sources = PreProcess(source);
        Compile(shader_sources);

        // Extract name from filepath
        auto last_slash = path.find_last_of("/\\");
        last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
        const auto last_dot = path.rfind('.');
        const auto count = last_dot == std::string::npos ? path.size() - last_slash : last_dot - last_slash;
        m_name = path.substr(last_slash, count);
    }

    OpenGLShader::~OpenGLShader()
    {
        HZ_PROFILE_FUNCTION();
        glDeleteProgram(m_renderer_id);
    }
    void OpenGLShader::Bind() const
    {
        HZ_PROFILE_FUNCTION();
        glUseProgram(m_renderer_id);
    }
    void OpenGLShader::UnBind() const
    {
        HZ_PROFILE_FUNCTION();
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string &name, const int value) const
    {
        HZ_PROFILE_FUNCTION();
        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) const
    {
        HZ_PROFILE_FUNCTION();
        UploadUniformMat4(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value) const
    {
        HZ_PROFILE_FUNCTION();
        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value) const
    {
        HZ_PROFILE_FUNCTION();
        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string &name, const int value) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform1i(location, value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string &name, const float value) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &value) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform2f(location, value.x, value.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &value) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }
    void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &value) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3 &matrix) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const
    {
        const GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
    }

    std::string OpenGLShader::ReadFile(const std::string &path)
    {
        HZ_PROFILE_FUNCTION();
        std::string result;
        if (std::ifstream in(path, std::ios::in, std::ios::binary); in)
        {
            // 获取文件长度
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(result.data(), static_cast<int64_t>(size));
                in.close();
            }
            else
            {
                HZ_CORE_ERROR("counld not read from file '{0}'", path);
            }
        }
        else
        {
            HZ_CORE_ERROR("Could not open file '{0}'", path);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string &source)
    {
        HZ_PROFILE_FUNCTION();
        // 格式如：#type Vertex ... #type Fragment ...

        std::unordered_map<GLenum, std::string> shader_sources;

        const auto *const type_token = "#type";

        const size_t type_token_length = strlen(type_token);
        // 第一个着色器类型声明行开头位置
        size_t pos = source.find(type_token, 0);

        while (pos != std::string::npos)
        {
            // 着色器类型声明行结尾位置
            const size_t eol = source.find_first_of("\r\n", pos);
            HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            // 着色器类型名位置（'#type '之后）
            const size_t begin = pos + type_token_length + 1;
            // 截取着色器类型名
            std::string type = source.substr(begin, eol - begin);
            HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            // 着色器类型声明行结尾位置
            const size_t next_line_pos = source.find_first_not_of("\r\n", eol);
            HZ_CORE_ASSERT(next_line_pos != std::string::npos, "Syntax error");
            // 下一个着色器类型声明行开头位置
            pos = source.find(type_token, next_line_pos);
            shader_sources[ShaderTypeFromString(type)] = pos == std::string::npos
                                                             ? source.substr(next_line_pos)
                                                             : source.substr(next_line_pos, pos - next_line_pos);
        }
        return shader_sources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> &shader_sources)
    {
        HZ_PROFILE_FUNCTION();
        const GLuint program = glCreateProgram();

        HZ_CORE_ASSERT(shader_sources.size() <= 2, "We only support 2 shaders for now");

        std::array<GLuint, 2> shader_ids{};

        int shader_id_index = 0;
        for (const auto &k_v : shader_sources)
        {
            // shade类型
            const GLenum type = k_v.first;
            // shade源码
            const std::string &source = k_v.second;
            // 创建shader
            const GLuint shader = glCreateShader(type);
            // 添加shade源码
            const GLchar *source_c_str = source.c_str();
            glShaderSource(shader, 1, &source_c_str, nullptr);
            // 编译shade
            glCompileShader(shader);
            // 编译错误处理，并跳出循环
            GLint is_compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE)
            {
                GLint max_length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
                std::vector<GLchar> info_log(max_length);
                glGetShaderInfoLog(shader, max_length, &max_length, info_log.data());
                glDeleteShader(shader);
                HZ_CORE_ERROR("{0}", info_log.data());
                HZ_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }
            // 编译完成附加到program
            glAttachShader(program, shader);
            // 保存已经编译的sharedId添加到数组
            shader_ids[shader_id_index++] = shader;
        }

        // 链接shader
        glLinkProgram(program);
        // 链接错误处理
        GLint is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE)
        {
            GLint max_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> info_log(max_length);
            glGetProgramInfoLog(program, max_length, &max_length, info_log.data());
            // link 失败：删除 program 删除 shader
            for (const auto id : shader_ids)
            {
                glDetachShader(program, id);
                glDeleteShader(id);
            }
            glDeleteProgram(program);
        }
        // shader编译链接到program后就可以删除了
        for (const auto id : shader_ids)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }
        m_renderer_id = program;
    }
}  // namespace Hazel