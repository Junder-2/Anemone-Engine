#pragma once

#include <vulkan/vulkan.h>
#include <slang-com-ptr.h>

namespace VulkanUtils
{
    void TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);

    void CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize);

    bool LoadShaderModule(const char* filePath, VkDevice device, const VkAllocationCallbacks* allocator, VkShaderModule* outShaderModule);

    bool LoadShaderModule(const Slang::ComPtr<slang::IBlob>& program, VkDevice device, const VkAllocationCallbacks* allocator, VkShaderModule* outShaderModule);

    void LoadSlangShaders(const char* moduleName, VkDevice device, const VkAllocationCallbacks* allocator, VkShaderModule* vertShader, VkShaderModule* fragShader);
}
