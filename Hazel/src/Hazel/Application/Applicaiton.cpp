#include "hzpch.h"
#include "Applicaiton.h"

namespace Hazel {
	Applicaiton::Applicaiton()
	{
		
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
		
		}
	}
}