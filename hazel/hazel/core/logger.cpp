
#include "hazel/core/logger.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Hazel
{
    std::shared_ptr<spdlog::logger> Logger::m_core_logger;
    std::shared_ptr<spdlog::logger> Logger::m_client_logger;
    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        m_core_logger = spdlog::stdout_color_mt("HAZEL");
        m_core_logger->set_level(spdlog::level::trace);
        m_client_logger = spdlog::stdout_color_mt("APP");
        m_client_logger->set_level(spdlog::level::trace);
    }
}  // namespace Hazel