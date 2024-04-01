#pragma once

#include <vulkan/vulkan.h>

namespace VulkanInitializers
{
    VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags);
    VkCommandBufferSubmitInfo CommandBufferSubmitInfo(VkCommandBuffer cmd);

    VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags);

    VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags);
    VkSemaphoreSubmitInfo SemaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);

    VkSubmitInfo2 SubmitInfo(const VkCommandBufferSubmitInfo* cmd, const VkSemaphoreSubmitInfo* signalSemaphoreInfo, const VkSemaphoreSubmitInfo* waitSemaphoreInfo);

    VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo ImageviewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);
}
