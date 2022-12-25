#include "platform/opengl/opengl_framebuffer.h"

#include <glad/glad.h>

namespace Hazel
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec) : m_spec(spec)
    {
        glCreateFramebuffers(1, &m_renderer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment);
        glBindTexture(GL_TEXTURE_2D, m_color_attachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(m_spec.Width),
                     static_cast<GLsizei>(m_spec.Height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
        glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_spec.Width),
                       static_cast<GLsizei>(m_spec.Height));

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

        HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                       "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer() { glDeleteFramebuffers(1, &m_renderer_id); }
    void OpenGLFrameBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id); }

    void OpenGLFrameBuffer::UnBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}  // namespace Hazel
