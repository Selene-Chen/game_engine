#pragma once
#include "Hazel/Window.h"
namespace Hazel
{
	class Applicaiton
	{
	public: 
		Applicaiton();
		virtual ~Applicaiton();
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
	};
	//to be define in client
	Applicaiton* CreateApplication();
}


