#pragma once
#include "hazel/core/application.h"
#include "hazel/core/core.h"

#ifdef HZ_PLATFORM_WINDOWS
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
    Hazel::Logger::Init();
    HZ_CORE_TRACE("Hello Hazel core loging");
    HZ_TRACE("Hello Hazel client loging");
    auto* const app = Hazel::CreateApplication();
    app->Run();
    delete app;
}
#endif