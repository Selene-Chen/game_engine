#pragma once

#include "hazel/core/window.h"
#include "hazel/renderer/graphics_context.h"

struct GLFWwindow;
namespace Hazel
{
    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(const WindowProps& props);
        ~WindowsWindow() override;

        void OnUpdate() override;

        [[nodiscard]] uint32_t GetWidth() const override { return m_win_data.Width; }
        [[nodiscard]] uint32_t GetHeight() const override { return m_win_data.Height; }
        void SetEventCallback(const EventCallbackFn& callback) override { m_win_data.EventCallback = callback; }

        void SetVSync(bool enabled) override;
        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] void* GetNativeWindow() const override { return m_window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

        GLFWwindow* m_window;
        Scope<GraphicsContext> m_context;
        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };
        WindowData m_win_data;
    };
}  // namespace Hazel