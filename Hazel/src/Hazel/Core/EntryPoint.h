#pragma once
#include "Hazel/Core/Log/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
    printf("Hello Hazel!\n");
    Hazel::Log::Init();
    HZ_CORE_TRACE("Hello Hazel core loging");
    HZ_TRACE("Hello Hazel client loging");

    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
}
#endif