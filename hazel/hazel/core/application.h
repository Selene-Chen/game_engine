#pragma once
#include "hazel/core/layer_stack.h"
#include "hazel/core/window.h"
#include "hazel/events/application_event.h"
#include "hazel/imgui/imgui_layer.h"

namespace Hazel
{
    // 单例
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;
        Application(const Application&) = delete;             // 删除拷贝构造函数
        Application& operator=(const Application&) = delete;  // 删除赋值操作

        void Run();
        void OnEvent(Event& event);

        void PushLayer(const Ref<Layer>& layer);
        void PushOverlay(const Ref<Layer>& overlay);

        static Application& GetInstance() { return *m_instance; }
        [[nodiscard]] Window& GetWindow() const { return *m_window; }

    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResizeEvent(WindowResizeEvent& event);

        Scope<Window> m_window;
        bool m_running = true;
        bool m_minimized = false;

        LayerStack m_layer_stack;
        Ref<ImGuiLayer> m_imgui_layer;

        float m_last_frame_time = 0.0F;

        static Application* m_instance;
    };
    // to be define in client
    Application* CreateApplication();
}  // namespace Hazel