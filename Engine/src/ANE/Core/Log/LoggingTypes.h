#pragma once
#include "ANE/Utilities/CommonUtilities.h"

namespace Engine
{
    typedef uint8 LogLevelCategories;
    typedef uint16 LoggerIndex;

    enum class LogLevelCategory : uint8
    {
        None = 0,
        Trace = BIT(0),
        Debug = BIT(1),
        Info = BIT(2),
        Warn = BIT(3),
        Error = BIT(4)
    };
    ENUM_CLASS_OPERATORS(LogLevelCategory);
}
