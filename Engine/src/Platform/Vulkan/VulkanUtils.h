#pragma once

#include <vulkan/vulkan.h>

namespace VulkanUtils
{
    void TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);

    void CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize);

    bool LoadShaderModule(const char* filePath, VkDevice device, VkShaderModule* outShaderModule);
}
