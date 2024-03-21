#pragma once

#ifdef NP_BUILD_DLL
    #define NP_API __declspec(dllexport)
#else
    #define NP_API __declspec(dllimport)
#endif
