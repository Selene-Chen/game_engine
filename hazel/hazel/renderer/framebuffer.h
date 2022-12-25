#pragma once
#include "hazel/core/core.h"

namespace Hazel
{
    struct FrameBufferSpec
    {
        uint32_t Width{}, Height{};
        uint32_t Samples     = 1;
        bool SwapChainTarget = false;
    };
    class FrameBuffer
    {
    public:
        FrameBuffer()          = default;
        virtual ~FrameBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        [[nodiscard]] virtual uint32_t GetColorAttachmentRendererID() const   = 0;
        [[nodiscard]] virtual const FrameBufferSpec& GetSpecification() const = 0;

        static Ref<FrameBuffer> Create(const FrameBufferSpec& spec);
    };
}  // namespace Hazel