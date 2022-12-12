#pragma once
#include <functional>

#include "hazel/events/event.h"

namespace Hazel
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        explicit WindowProps(std::string title = "Hazel Engine", uint32_t width = 1280, uint32_t height = 720)
            : Title(std::move(title)), Width(width), Height(height)
        {}
    };
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        virtual ~Window() = default;

        [[nodiscard]] virtual unsigned int GetWidth() const = 0;
        [[nodiscard]] virtual unsigned int GetHeight() const = 0;
        [[nodiscard]] virtual bool IsVSync() const = 0;
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;
        virtual void OnUpdate() = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;

        static Scope<Window> Create(const WindowProps& prop = WindowProps());
    };
}  // namespace Hazel