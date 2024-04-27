#pragma once

#define CHECK_RESULT(x) ANE_MACRO_SCOPE( \
{ \
    VkResult err = (x); \
    if (err != VK_SUCCESS) { \
        if (err < 0) ANE_EASSERT(false, "VkResult is negative ({})", (int)err); \
        else ANE_ELOG_WARN("Vulkan Warning: VkResult is not VK_SUCCESS ({})", (int)err); \
    } \
})
