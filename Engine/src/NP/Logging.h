#pragma once

#include <memory>

#include "Core.h"

//solves warnings
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Engine
{    
    class NP_API Logging
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return engineLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return appLogger; }
    private:
        static std::shared_ptr<spdlog::logger> engineLogger;
        static std::shared_ptr<spdlog::logger> appLogger;
    };   
}

// Engine log macros
#define NP_ENGINE_LOG_TRACE(...)    ::Engine::Logging::GetEngineLogger()->trace(__VA_ARGS__)
#define NP_ENGINE_LOG_INFO(...)     ::Engine::Logging::GetEngineLogger()->info(__VA_ARGS__)
#define NP_ENGINE_LOG_WARN(...)     ::Engine::Logging::GetEngineLogger()->warn(__VA_ARGS__)
#define NP_ENGINE_LOG_ERROR(...)    ::Engine::Logging::GetEngineLogger()->error(__VA_ARGS__)
#define NP_ENGINE_LOG_CRITICAL(...) ::Engine::Logging::GetEngineLogger()->critical(__VA_ARGS__)

// App log macros
#define NP_LOG_TRACE(...)         ::Engine::Logging::GetAppLogger()->trace(__VA_ARGS__)
#define NP_LOG_INFO(...)          ::Engine::Logging::GetAppLogger()->info(__VA_ARGS__)
#define NP_LOG_WARN(...)          ::Engine::Logging::GetAppLogger()->warn(__VA_ARGS__)
#define NP_LOG_ERROR(...)         ::Engine::Logging::GetAppLogger()->error(__VA_ARGS__)
#define NP_LOG_CRITICAL(...)      ::Engine::Logging::GetAppLogger()->critical(__VA_ARGS__)
