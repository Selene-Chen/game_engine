#pragma once
#include "Hazel/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Layer/Layerstack.h"

namespace Hazel
{
	class Application
	{
	public: 
		Application();
		virtual ~Application();
		void OnEvent(Event& e);
		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		inline static Application& GetInstance() { return *s_instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		static Application* s_instance;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};
	//to be define in client
	Application* CreateApplication();
}
