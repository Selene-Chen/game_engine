#pragma once
#include <string>

namespace Hazel
{
    class Texture
    {
    public:
        Texture()          = default;
        virtual ~Texture() = default;

    public:
        virtual uint32_t GetWidth() const  = 0;
        virtual uint32_t GetHeight() const = 0;

    public:
        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& path);
    };
} // namespace Hazel