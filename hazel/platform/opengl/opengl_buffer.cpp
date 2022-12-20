#include "platform/opengl/opengl_buffer.h"

#include "glad/glad.h"  // IWYU pragma: export
#include "hzpch.h"
namespace Hazel
{

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32_t size)
    {
        HZ_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }
    OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32_t size)
    {
        HZ_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_renderer_id);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        HZ_PROFILE_FUNCTION();
        
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    }

    void OpenGLVertexBuffer::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count) : m_count(count)
    {
        HZ_PROFILE_FUNCTION();
        glCreateBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices,
                     GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        HZ_PROFILE_FUNCTION();
        glDeleteBuffers(1, &m_renderer_id);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        HZ_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    }

    void OpenGLIndexBuffer::UnBind() const
    {
        HZ_PROFILE_FUNCTION();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}  // namespace Hazel