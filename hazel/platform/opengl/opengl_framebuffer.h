#pragma once

#include "hazel/renderer/framebuffer.h"

namespace Hazel
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        explicit OpenGLFrameBuffer(const FrameBufferSpec& spec);
        ~OpenGLFrameBuffer() override;

        void Bind() const override;
        void UnBind() const override;

        [[nodiscard]] uint32_t GetColorAttachmentRendererID() const override { return m_color_attachment; }
        [[nodiscard]] const FrameBufferSpec& GetSpecification() const override { return m_spec; }

    private:
        uint32_t m_renderer_id = 0, m_depth_attachment = 0, m_color_attachment = 0;
        FrameBufferSpec m_spec;
    };
}  // namespace Hazel