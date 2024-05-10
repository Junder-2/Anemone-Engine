#pragma once

namespace Engine
{
    namespace LoggingUtilities
    {
        inline std::string ToString(const LogLevelCategory type)
        {
            switch (type)
            {
                case LogLevelCategory::Trace: return "Trace";
                case LogLevelCategory::Debug: return "Debug";
                case LogLevelCategory::Info: return "Info";
                case LogLevelCategory::Warn: return "Warn";
                case LogLevelCategory::Error: return "Error";
                default: return "Unknown";
            }
        }
    }
}
