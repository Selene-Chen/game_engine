#include "platform/opengl/opengl_buffer.h"

#include "glad/glad.h"  // IWYU pragma: export

namespace Hazel
{

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, const uint32_t size)
    {
        glCreateBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

    void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id); }

    void OpenGLVertexBuffer::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count) : m_count(count)
    {
        glCreateBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices,
                     GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

    void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id); }

    void OpenGLIndexBuffer::UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}  // namespace Hazel