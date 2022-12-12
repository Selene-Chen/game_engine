#pragma once

#include <spdlog/spdlog.h>

namespace Hazel
{
    class Logger
    {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_core_logger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_client_logger; }

    private:
        static std::shared_ptr<spdlog::logger> m_core_logger;
        static std::shared_ptr<spdlog::logger> m_client_logger;
    };
}  // namespace Hazel

#define HZ_CORE_TRACE(...)    ::Hazel::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)     ::Hazel::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)     ::Hazel::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)    ::Hazel::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_CRITICAL(...) ::Hazel::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define HZ_TRACE(...)    ::Hazel::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)     ::Hazel::Logger::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)     ::Hazel::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)    ::Hazel::Logger::GetClientLogger()->error(__VA_ARGS__)
#define HZ_ERROR(...)    ::Hazel::Logger::GetClientLogger()->error(__VA_ARGS__)
#define HZ_CRITICAL(...) ::Hazel::Logger::GetClientLogger()->critical(__VA_ARGS__)