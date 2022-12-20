#pragma once
#include <string>

#include "hazel/core/core.h"

namespace Hazel
{
    class Texture
    {
    public:
        Texture()          = default;
        virtual ~Texture() = default;

        [[nodiscard]] virtual uint32_t GetWidth() const  = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        virtual void Bind(uint32_t slot) const                = 0;
        virtual void SetData(void* data, uint32_t size) const = 0;

        virtual bool operator==(const Texture& other) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D()           = default;
        ~Texture2D() override = default;

        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const std::string& path);
    };
}  // namespace Hazel