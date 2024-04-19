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

#include "ANE/Core/Log/Logging.h"
#include "ANE/Core/Assert.h"

#include "ANE/Debug/Instrumentation.h"

typedef enum
{
    ANENoError = 0,
} ANEErrorCode;
