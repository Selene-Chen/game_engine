#pragma once
#include "hazel/renderer/vertex_array.h"

namespace Hazel
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void Bind() const override;
        void UnBind() const override;
        void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;
        [[nodiscard]] std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_vertex_buffers; }
        [[nodiscard]] Ref<IndexBuffer> GetIndexBuffer() const override { return m_index_buffer; }

    private:
        std::vector<Ref<VertexBuffer>> m_vertex_buffers;
        Ref<IndexBuffer> m_index_buffer;
        uint32_t m_renderer_id = 0;
        uint32_t m_vertex_buffer_index = 0;
    };
}  // namespace Hazel