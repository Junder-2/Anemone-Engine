#include "nppch.h"
#include "Logging.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
    std::shared_ptr<spdlog::logger> Logging::_engineLogger;
    std::shared_ptr<spdlog::logger> Logging::_appLogger;

    void Logging::InitEngine()
    {
        #ifndef NP_DIST
        spdlog::set_pattern("%^[%n %T] %v%$");

        _engineLogger = spdlog::stdout_color_mt("ENGINE");
        _engineLogger->set_level(spdlog::level::trace);
        #endif
    }

    void Logging::InitApp(const std::string& appName)
    {
        #ifndef NP_DIST
        _appLogger = spdlog::stdout_color_mt(appName);
        _appLogger->set_level(spdlog::level::trace);
        #endif
    }
}
