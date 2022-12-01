#pragma once
namespace Hazel
{
	class Applicaiton
	{
	public: 
		Applicaiton();
		virtual ~Applicaiton();
		void Run();
	};
	//to be define in client
	Applicaiton* CreateApplication();
}


