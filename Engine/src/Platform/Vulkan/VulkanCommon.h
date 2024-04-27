#pragma once

#define CHECK_RESULT(err) ANE_MACRO_SCOPE( \
{ \
    if ((err) != VK_SUCCESS) { \
        if ((err) < 0) ANE_EASSERT(false, "VkResult is negative ({})", (int)(err)); \
        else ANE_ELOG_WARN("Vulkan Warning: VkResult is not VK_SUCCESS ({})", (int)(err)); \
    } \
})
