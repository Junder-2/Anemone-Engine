#pragma once
#include "ANE/Utilities/CommonUtilities.h"

namespace Engine
{
    enum class LogLevelCategory
    {
        LevelNone = 0,
        LevelTrace = BIT(0),
        LevelDebug = BIT(1),
        LevelInfo = BIT(2),
        LevelWarn = BIT(3),
        LevelError = BIT(4)
    };

    //log_msg
    struct LogMessage
    {
        std::string Time;
        std::string Source;
        std::string LoggerName;
        LogLevelCategory LevelCategory;
        std::string Message;
    };
}
