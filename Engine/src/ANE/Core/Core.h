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

#include "Logging.h"

typedef enum
{
    ANENoError = 0,
} ANEErrorCode;
