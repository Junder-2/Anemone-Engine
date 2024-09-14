#pragma once

#if ANE_DYNAMIC_LINK
    #ifdef ANE_BUILD_DLL
        #define ANE_API __declspec(dllexport)
    #else
        #define ANE_API __declspec(dllimport)
    #endif
#else
    #define ANE_API
#endif

// "do {x} while (0)" scopes the macro content and forces usages to end with semi-colon.
#define ANE_MACRO_SCOPE(macro) do { macro } while (0)

namespace Engine
{
    using int8 = signed char;
    using int16 = short;
    using int64 = long long;

    using uint8 = unsigned char;
    using uint16 = unsigned short;
    using uint = unsigned int;
    using uint64 = unsigned long long;

    /** Shortened static_cast<float> */
    inline float ToFloat(auto value) { return static_cast<float>(value); }

    /** Shortened static_cast<int8> */
    inline int8 ToInt8(auto value) { return static_cast<int8>(value); }
    /** Shortened static_cast<uint8> */
    inline uint8 ToUInt8(auto value) { return static_cast<uint8>(value); }
    /** Shortened static_cast<int16> */
    inline int16 ToInt16(auto value) { return static_cast<int16>(value); }
    /** Shortened static_cast<uint16> */
    inline uint16 ToUInt16(auto value) { return static_cast<uint16>(value); }
    /** Shortened static_cast<int> */
    inline int ToInt(auto value) { return static_cast<int>(value); }
    /** Shortened static_cast<uint> */
    inline uint ToUInt(auto value) { return static_cast<uint>(value); }
    /** Shortened static_cast<int64> */
    inline int64 ToInt64(auto value) { return static_cast<int64>(value); }
    /** Shortened static_cast<uint64> */
    inline uint64 ToUInt64(auto value) { return static_cast<uint64>(value); }
}

#include "ANE/Core/Log/Logging.h"
#include "ANE/Core/Assert.h"

#include "ANE/Math/MathCore.h"

#include "ANE/Debug/Instrumentation.h"

typedef enum
{
    ANENoError = 0,
} ANEErrorCode;
