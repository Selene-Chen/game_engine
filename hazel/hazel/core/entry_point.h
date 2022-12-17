#pragma once
#include "hazel/core/application.h"
#include "hazel/core/core.h"
#include "hazel/debug/instrumentor.h"

#ifdef HZ_PLATFORM_WINDOWS
extern Hazel::Application* Hazel::CreateApplication();

int main(int /*argc*/, char** /*argv*/)
{
    Hazel::Logger::Init();

    HZ_PROFILE_BEGIN_SESSION("stratup", "HazelProfile-Startup.json");
    auto* const app = Hazel::CreateApplication();
    HZ_PROFILE_END_SESSION();
    HZ_PROFILE_BEGIN_SESSION("runtime", "HazelProfile-Runtime.json");
    app->Run();
    HZ_PROFILE_END_SESSION();
    HZ_PROFILE_BEGIN_SESSION("shut-down", "HazelProfile-Shutdown.json");
    delete app;
    HZ_PROFILE_END_SESSION();
}
#endif