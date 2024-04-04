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

    VkRenderingAttachmentInfo AttachmentInfo(VkImageView view, const VkClearValue* clear, VkImageLayout layout);
    VkRenderingAttachmentInfo DepthAttachmentInfo(VkImageView view, VkImageLayout layout);

    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule, const char* entry = "main");

    VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo ImageviewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);
}
