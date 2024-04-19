#pragma once

#include "Core.h"
#include "Log/Logging.h"

#ifdef ANE_DEBUG

    #define ANE_DEBUG_BREAK() __debugbreak()

    #define ANE_INTERNAL_ASSERT_IMPL(logger, cond, ...) ANE_MACRO_SCOPE( if(!(cond)) { logger("Assert failed: {0}", __VA_ARGS__); ANE_DEBUG_BREAK(); } )

    #define ANE_EASSERT(cond, ...) ANE_INTERNAL_ASSERT_IMPL(ANE_ELOG_ERROR, cond, __VA_ARGS__)
    #define ANE_ASSERT(cond, ...) ANE_INTERNAL_ASSERT_IMPL(ANE_LOG_ERROR, cond, __VA_ARGS__)
#else
    #define ANE_ASSERT(cond, ...)
    #define ANE_EASSERT(cond, ...)
    #define ANE_DEBUG_BREAK()
#endif
