#pragma once

#if NP_DYNAMIC_LINK
    #ifdef NP_BUILD_DLL
        #define NP_API __declspec(dllexport)
    #else
        #define NP_API __declspec(dllimport)
    #endif
#else
    #define NP_API
#endif

#define BIT(x) (1 << x)
#define TWO_BIT_MASK(x) ((0b11 << ((x)*2)))

#include "Logging.h"

typedef enum
{
    NPNoError = 0,
} NPErrorCode;
