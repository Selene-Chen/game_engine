#include "hzpch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

namespace Hazel
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
        HZ_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }
    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(sources);
    }

    OpenGLShader::OpenGLShader(const std::string& path)
    {
        auto source = ReadFile(path);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        // Extract name from filepath
        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = path.rfind('.');
        auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        m_Name = path.substr(lastSlash, count);
    }

    OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }
    void OpenGLShader::Bind() const { glUseProgram(m_RendererID); }
    void OpenGLShader::UnBind() const { glUseProgram(0); }
    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::ReadFile(const std::string& path)
    {
        std::string result;
        std::ifstream in(path, std::ios::in, std::ios::binary);
        if (in)
            {
                // 获取文件长度
                in.seekg(0, std::ios::end);
                result.resize(in.tellg());
                // 读取文件
                in.seekg(0, std::ios::beg);
                in.read(&result[0], result.size());
                // 关闭文件
                in.close();
            }
        else
            {
                HZ_CORE_ERROR("Could not open file '{0}'", path);
            }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        // 格式如：#type Vertex ... #type Fragment ...

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";

        size_t typeTokenLength = strlen(typeToken);
        // 第一个着色器类型声明行开头位置
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
            {
                // 着色器类型声明行结尾位置
                size_t eol = source.find_first_of("\r\n", pos);
                HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
                // 着色器类型名位置（'#type '之后）
                size_t begin = pos + typeTokenLength + 1;
                // 截取着色器类型名
                std::string type = source.substr(begin, eol - begin);
                HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

                // 着色器类型声明行结尾位置
                size_t nextLinePos = source.find_first_not_of("\r\n", eol);
                HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
                // 下一个着色器类型声明行开头位置
                pos = source.find(typeToken, nextLinePos);
                shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos)
                                                                ? source.substr(nextLinePos)
                                                                : source.substr(nextLinePos, pos - nextLinePos);
            }
        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();

        HZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
        std::array<GLuint, 2> glShaderIDs;

        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
            {
                // shade类型
                GLenum type = kv.first;
                // shade源码
                const std::string& source = kv.second;
                // 创建shader
                GLuint shader = glCreateShader(type);
                // 添加shade源码
                const GLchar* sourceCStr = source.c_str();
                glShaderSource(shader, 1, &sourceCStr, 0);
                // 编译shade
                glCompileShader(shader);
                // 编译错误处理，并跳出循环
                GLint isCompiled = 0;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE)
                    {
                        GLint maxLength = 0;
                        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                        std::vector<GLchar> infoLog(maxLength);
                        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                        glDeleteShader(shader);
                        HZ_CORE_ERROR("{0}", infoLog.data());
                        HZ_CORE_ASSERT(false, "Shader compilation failure!");
                        break;
                    }
                // 编译完成附加到program
                glAttachShader(program, shader);
                // 保存已经编译的sharedId添加到数组
                glShaderIDs[glShaderIDIndex++] = shader;
            }

        // 链接shader
        glLinkProgram(program);
        // 链接错误处理
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> infoLog(maxLength);
                glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
                // link 失败：删除 program 删除 shader
                for (auto id : glShaderIDs)
                    {
                        glDetachShader(program, id);
                        glDeleteShader(id);
                    }
                glDeleteProgram(program);
            }
        // shader编译链接到program后就可以删除了
        for (auto id : glShaderIDs)
            {
                glDetachShader(program, id);
                glDeleteShader(id);
            }

        m_RendererID = program;
    }
}  // namespace Hazel