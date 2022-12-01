#pragma once
#ifdef HZ_PLATFORM_WINDOWS
extern Hazel::Applicaiton* Hazel::CreateApplication();

int main(int argc,char** argv)
{
	printf("Hello Hazel!");
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}
#endif