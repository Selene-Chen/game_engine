#include "hzpch.h"
#include "Applicaiton.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "glfw/glfw3.h"

namespace Hazel {
	Applicaiton::Applicaiton()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Applicaiton::~Applicaiton()
	{

	}
	
	void Applicaiton::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			HZ_TRACE(e.ToString());
		}

		while (true)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}