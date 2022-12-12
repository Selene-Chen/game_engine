#pragma once

#include "hazel/renderer/buffer.h"

namespace Hazel
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void UnBind() const override;
        void SetLayout(const BufferLayout& layout) override { m_layout = layout; }
        [[nodiscard]] const BufferLayout& GetLayout() const override { return m_layout; }

    private:
        uint32_t m_renderer_id = 0;
        BufferLayout m_layout;
    };
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        [[nodiscard]] uint32_t GetCount() const override { return m_count; }
        void Bind() const override;
        void UnBind() const override;

    private:
        uint32_t m_renderer_id = 0;
        uint32_t m_count;
    };
}  // namespace Hazel