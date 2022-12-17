
#include "opengl_vertex_array.h"

#include "glad/glad.h"  // IWYU pragma: export
#include "hzpch.h"
namespace Hazel
{
    static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
            default:;
        }
        HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_renderer_id);
        glBindVertexArray(m_renderer_id);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        HZ_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &m_renderer_id);
    }

    void OpenGLVertexArray::Bind() const
    {
        HZ_PROFILE_FUNCTION();
        glBindVertexArray(m_renderer_id);
    }

    void OpenGLVertexArray::UnBind() const
    {
        HZ_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
    {
        HZ_PROFILE_FUNCTION();
        HZ_CORE_ASSERT(!vertex_buffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_renderer_id);
        vertex_buffer->Bind();
        const auto& layout = vertex_buffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(m_vertex_buffer_index);
            glVertexAttribPointer(m_vertex_buffer_index, static_cast<GLint>(element.GetComponentCout()),
                                  ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLint>(layout.GetStride()),
                                  reinterpret_cast<const void*>(element.Offset));
            m_vertex_buffer_index++;
        }
        m_vertex_buffers.push_back(vertex_buffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
    {
        HZ_PROFILE_FUNCTION();
        glBindVertexArray(m_renderer_id);
        index_buffer->Bind();
        m_index_buffer = index_buffer;
    }

}  // namespace Hazel