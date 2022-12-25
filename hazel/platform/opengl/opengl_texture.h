#pragma once

#include "hazel/renderer/texture.h"

namespace Hazel
{
    // TODO(Selene): should be remove
    using GLenum = unsigned int;
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        explicit OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D() override;

        [[nodiscard]] uint32_t GetWidth() const override { return m_width; }
        [[nodiscard]] uint32_t GetHeight() const override { return m_height; }
        [[nodiscard]] uint32_t GetRendererID() const override { return m_renderer_id; }

        void Bind(uint32_t slot) const override;
        void SetData(void* data, uint32_t size) const override;
        bool operator==(const Texture& other) const override
        {
            return m_renderer_id == (dynamic_cast<const OpenGLTexture2D&>(other)).m_renderer_id;
        }

    private:
        std::string m_path;
        uint32_t m_width, m_height;
        uint32_t m_renderer_id   = 0;
        GLenum m_internal_format = 0, m_data_format = 0;
    };
}  // namespace Hazel