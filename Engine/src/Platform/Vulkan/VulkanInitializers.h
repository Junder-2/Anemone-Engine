#pragma once

#include <vulkan/vulkan.h>

#include "VmaTypes.h"

namespace VulkanInitializers
{
    // Vulkan
    VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags);
    VkCommandBufferSubmitInfo CommandBufferSubmitInfo(VkCommandBuffer cmd);

    VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags);

    VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags);
    VkSemaphoreSubmitInfo SemaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);

    VkSubmitInfo2 SubmitInfo(const VkCommandBufferSubmitInfo* cmd, const VkSemaphoreSubmitInfo* signalSemaphoreInfo, const VkSemaphoreSubmitInfo* waitSemaphoreInfo);

    VkRenderingAttachmentInfo AttachmentInfo(VkImageView view, const VkClearValue* clear, VkImageLayout layout);
    VkRenderingAttachmentInfo DepthAttachmentInfo(VkImageView view, VkImageLayout layout);

    VkRenderingInfo RenderingInfo(VkExtent2D renderExtent, const VkRenderingAttachmentInfo* colorAttachment, const VkRenderingAttachmentInfo* depthAttachment);

    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule, const char* entry = "main");
    VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();

    VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo ImageviewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);

    // VMA

}
