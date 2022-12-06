#include "hzpch.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include "Application.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!s_instance, "Application aready exists!")
        s_instance = this;
        m_Window   = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_HZ_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);
    }

    void Application::OnEvent(Event& e)
    {
        // 输出当前事件信息
        // HZ_CORE_TRACE("{0}", e.ToString());
        // 设置事件调度处理函数，这里是关闭窗口就结束程序
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_HZ_EVENT_FN(Application::OnWindowClose));
        // 处理层事件
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float    time     = glfwGetTime(); // GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime   = time;

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            // auto [x, y] = Input::GetMousePosition();
            // HZ_CORE_TRACE("{0},{1}", x, y);

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }

    void Application::PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }
} // namespace Hazel