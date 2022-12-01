#pragma once
#include "Hazel/Window.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel
{
	class Application
	{
	public: 
		Application();
		virtual ~Application();
		void OnEvent(Event& e);
		void Run();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	//to be define in client
	Application* CreateApplication();
}


