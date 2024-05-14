#pragma once

const char* const ASSET_PATH_MESHES = "../Assets/Meshes/";
const char* const ASSET_PATH_SHADERS = "../Assets/Shaders/";
const char* const ASSET_PATH_TEXTURES = "../Assets/Textures/";

#define CHECK_RESULT(x) ANE_MACRO_SCOPE( \
{ \
    VkResult __err = (x); \
    if (__err != VK_SUCCESS) { \
        if (__err < 0) ANE_EASSERT(false, "VkResult is negative ({})", (int)__err); \
        else ANE_ELOG_WARN("Vulkan Warning: VkResult is not VK_SUCCESS ({})", (int)__err); \
    } \
})
