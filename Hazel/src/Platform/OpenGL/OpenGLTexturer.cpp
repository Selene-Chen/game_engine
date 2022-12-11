#include "hzpch.h"

#include "OpenGLTexturer.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Hazel
{

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        HZ_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        // texture channels
        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
        else if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
        HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        // 创建纹理
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        // 存储纹理
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
        // 纹理参数
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小过滤
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//放大过滤
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);//纹理包裹-repeat
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);//纹理包裹-repeat
        // 指定纹理
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

    void OpenGLTexture2D::Bind(uint32_t slot /*= 0*/) const { glBindTextureUnit(slot, m_RendererID); }

}  // namespace Hazel