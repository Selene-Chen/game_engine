#include "hzpch.h"
#include <glad/glad.h>
#include "Application.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
    Application*  Application::s_instance = nullptr;
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3: return GL_FLOAT;
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
        }
        HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }
 
    Application::Application() 
    {
        HZ_CORE_ASSERT(!s_instance, "Application aready exists!")
        s_instance = this;
        m_Window   = std::unique_ptr<Window>(Window::Create());
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
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

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