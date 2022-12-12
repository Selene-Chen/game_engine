#include "platform/opengl/opengl_texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Hazel
{

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_path(path)
    {
        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        HZ_CORE_ASSERT(data, "Failed to load image!");
        m_width = width;
        m_height = height;

        // texture channels
        GLenum internal_format = 0;
        GLenum data_format = 0;

        if (channels == 4)
        {
            internal_format = GL_RGBA8;
            data_format = GL_RGBA;
        }
        else if (channels == 3)
        {
            internal_format = GL_RGB8;
            data_format = GL_RGB;
        }
        HZ_CORE_ASSERT(internal_format & data_format, "Format not supported!");

        m_internal_format = internal_format;
        m_internal_format = data_format;

        // 创建纹理
        glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
        // 存储纹理
        glTextureStorage2D(m_renderer_id, 1, internal_format, static_cast<int>(m_width), static_cast<int>(m_height));
        // 纹理参数
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // 缩小过滤
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // 放大过滤
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);       // 纹理包裹-repeat
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);       // 纹理包裹-repeat
        // 指定纹理
        glTextureSubImage2D(m_renderer_id, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), data_format,
                            GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
        : m_width(width), m_height(height), m_internal_format(GL_RGBA8), m_data_format(GL_RGBA)
    {
        // 创建纹理
        glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
        // 存储纹理
        glTextureStorage2D(m_renderer_id, 1, m_internal_format, static_cast<int>(m_width), static_cast<int>(m_height));
        // 纹理参数
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_renderer_id); }

    void OpenGLTexture2D::Bind(const uint32_t slot) const { glBindTextureUnit(slot, m_renderer_id); }

    void OpenGLTexture2D::SetData(void* data, const uint32_t /*size*/) const
    {
        HZ_CORE_ASSERT(m_width * m_width * (m_data_format == GL_RGBA ? 4 : 3), "Data must be entire texture!");
        glTextureSubImage2D(m_renderer_id, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height),
                            m_data_format, GL_UNSIGNED_BYTE, data);
    }

}  // namespace Hazel