#pragma once
#include <vector>

#include "hazel/renderer/buffer.h"

namespace Hazel
{
    class VertexArray
    {
    public:
        VertexArray() = default;
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

        [[nodiscard]] virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const = 0;
        [[nodiscard]] virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };
}  // namespace Hazel