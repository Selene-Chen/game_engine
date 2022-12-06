#pragma once
#include "Hazel/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Layer/Layerstack.h"
#include "Hazel/Imgui/ImguiLayer.h"
#include "Hazel/Input.h"

#include "Hazel/Renderer/Shader.h"
#include "hazel/Renderer/Buffer.h"
#include "hazel/Renderer/VertexArray.h"

#include "Hazel/Renderer/OrthographicCamera.h"
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
        bool                    OnWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;
        bool                    m_Running = true;
        LayerStack              m_LayerStack;
        ImGuiLayer*             m_ImGuiLayer;
        // 三角形
        std::shared_ptr<Shader>      m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;
        // 正方形
        std::shared_ptr<VertexArray> m_SquareVertexArray;
        std::shared_ptr<Shader>      m_SquareShader;
        OrthographicCamera           m_Camera;

    private:
        static Application* s_instance;
    };
    // to be define in client
    Application* CreateApplication();
} // namespace Hazel
