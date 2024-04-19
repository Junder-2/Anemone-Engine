#pragma once

#include "Core.h"
#include "Log/Logging.h"

#ifdef ANE_DEBUG

    #define ANE_DEBUG_BREAK() __debugbreak()

    #define ANE_INTERNAL_ASSERT_IMPL(logger, cond, msg) ANE_MACRO_SCOPE( if(!(cond)) { logger("Assert failed: {0}", msg); ANE_DEBUG_BREAK(); } )

    #define ANE_EASSERT(cond, msg) ANE_INTERNAL_ASSERT_IMPL(ANE_ELOG_ERROR, cond, msg)
    #define ANE_ASSERT(cond, msg) ANE_INTERNAL_ASSERT_IMPL(ANE_LOG_ERROR, cond, msg)
#else
    #define ANE_ASSERT(cond, msg)
    #define ANE_EASSERT(cond, msg)
    #define ANE_DEBUG_BREAK()
#endif
