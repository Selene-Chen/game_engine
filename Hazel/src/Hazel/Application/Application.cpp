#include "hzpch.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>
#include "Application.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!s_instance, "Application aready exists!")
        s_instance = this;
        m_Window   = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init(); // 初始化 Renderer

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);
    }

    void Application::OnEvent(Event& e)
    {
        // 输出当前事件信息
        // HZ_CORE_TRACE("{0}", e.ToString());
        // 设置事件调度处理函数，这里是关闭窗口就结束程序
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResizeEvent));
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

    bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
    {

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        return false;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float    time     = (float)glfwGetTime(); // GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime   = time;

            // 窗口最小化
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
      

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }

    void Application::PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }
} // namespace Hazel