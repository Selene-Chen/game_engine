#include "hzpch.h"
#include "Application.h"
#include <glad/glad.h>

namespace Hazel {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_instance,"Application aready exists!")
		s_instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_HZ_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
		
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
			if (e.Handled) break;
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
	
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//auto [x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0},{1}", x, y);

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}