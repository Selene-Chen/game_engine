#pragma once
#include "Hazel/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Layer/Layerstack.h"
#include "Hazel/Imgui/ImguiLayer.h"
#include "Hazel/Input.h"

namespace Hazel
{
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;
        void                       Run();
        void                       OnEvent(Event& e);
        void                       PushLayer(Layer* layer);
        void                       PushOverlay(Layer* overlay);
        inline static Application& GetInstance() { return *s_instance; }
        inline Window&             GetWindow() { return *m_Window; }

    private:
        bool          OnWindowClose(WindowCloseEvent& e);
        Scope<Window> m_Window;
        bool          m_Running = true;
        LayerStack    m_LayerStack;
        ImGuiLayer*   m_ImGuiLayer;
        float         m_LastFrameTime = 0.0f;

    private:
        static Application* s_instance;
    };
    // to be define in client
    Application* CreateApplication();
} // namespace Hazel
