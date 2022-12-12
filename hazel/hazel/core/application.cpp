#include "hazel/core/application.h"

#include <glfw/glfw3.h>

#include "hazel/core/timestep.h"
#include "hazel/renderer/renderer.h"

namespace Hazel
{
    Application* Application::m_instance = nullptr;

    Application::Application() : m_imgui_layer(CreateRef<ImGuiLayer>())
    {
        // 单例
        HZ_CORE_ASSERT(!m_instance, "Application aready exists!")
        m_instance = this;
        // 1.window
        m_window = Window::Create();
        m_window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
        // 2.renderer
        Renderer::Init();  // 初始化 Renderer
        PushLayer(m_imgui_layer);
    }

    void Application::OnEvent(Event& event)
    {
        // 输出当前事件信息
        // HZ_CORE_TRACE("{0}", e.ToString());
        // 设置事件调度处理函数，这里是关闭窗口就结束程序
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResizeEvent));
        // 处理层事件
        for (auto it = m_layer_stack.rbegin(); it != m_layer_stack.rend(); ++it)
        {
            (*it)->OnEvent(event);
            if (event.IsHandled())
            {
                break;
            }
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& /*event*/)
    {
        m_running = false;
        return true;
    }

    bool Application::OnWindowResizeEvent(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_minimized = true;
            return false;
        }
        m_minimized = false;
        // 改变视口
        Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
        return false;
    }

    void Application::Run()
    {
        while (m_running)
        {
            auto time = static_cast<float>(glfwGetTime());  // GetTime();
            auto timestep = Timestep(time - m_last_frame_time);
            m_last_frame_time = time;

            // 窗口最小化
            if (!m_minimized)
            {
                for (Ref<Layer>& layer : m_layer_stack)
                {
                    layer->OnUpdate(timestep);
                }
            }
            m_imgui_layer->Begin();
            for (Ref<Layer>& layer : m_layer_stack)
            {
                layer->OnImGuiRender();
            }
            m_imgui_layer->End();

            m_window->OnUpdate();
        }
    }

    void Application::PushLayer(const Ref<Layer>& layer) { m_layer_stack.PushLayer(layer); }

    void Application::PushOverlay(const Ref<Layer>& overlay) { m_layer_stack.PushOverlay(overlay); }
}  // namespace Hazel
