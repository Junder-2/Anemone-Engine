#include "anepch.h"
#include "VulkanInitializers.h"

namespace VulkanInitializers
{
    VkCommandBufferBeginInfo CommandBufferBeginInfo(const VkCommandBufferUsageFlags flags)
    {
        VkCommandBufferBeginInfo info = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .pNext = nullptr };

        info.pInheritanceInfo = nullptr;
        info.flags = flags;

        return info;
    }

    VkCommandBufferSubmitInfo CommandBufferSubmitInfo(const VkCommandBuffer cmd)
    {
        VkCommandBufferSubmitInfo info = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO, .pNext = nullptr };

        info.commandBuffer = cmd;
        info.deviceMask = 0;

        return info;
    }

    VkFenceCreateInfo FenceCreateInfo(const VkFenceCreateFlags flags)
    {
        VkFenceCreateInfo info = { info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, info.pNext = nullptr };

        info.flags = flags;

        return info;
    }

    VkSemaphoreCreateInfo SemaphoreCreateInfo(const VkSemaphoreCreateFlags flags)
    {
        VkSemaphoreCreateInfo info = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = nullptr };

        info.flags = flags;

        return info;
    }

    VkSemaphoreSubmitInfo SemaphoreSubmitInfo(const VkPipelineStageFlags2 stageMask, const VkSemaphore semaphore)
    {
        VkSemaphoreSubmitInfo submitInfo = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO, .pNext = nullptr };

        submitInfo.semaphore = semaphore;
        submitInfo.stageMask = stageMask;
        submitInfo.deviceIndex = 0;
        submitInfo.value = 1;

        return submitInfo;
    }

    VkSubmitInfo2 SubmitInfo(const VkCommandBufferSubmitInfo* cmd, const VkSemaphoreSubmitInfo* signalSemaphoreInfo, const VkSemaphoreSubmitInfo* waitSemaphoreInfo)
    {
        VkSubmitInfo2 info = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2, .pNext = nullptr };

        info.waitSemaphoreInfoCount = waitSemaphoreInfo == nullptr ? 0u : 1u;
        info.pWaitSemaphoreInfos = waitSemaphoreInfo;

        info.signalSemaphoreInfoCount = signalSemaphoreInfo == nullptr ? 0u : 1u;
        info.pSignalSemaphoreInfos = signalSemaphoreInfo;

        info.commandBufferInfoCount = 1;
        info.pCommandBufferInfos = cmd;

        return info;
    }

    VkRenderingAttachmentInfo AttachmentInfo(const VkImageView view, const VkClearValue* clear, const VkImageLayout layout)
    {
        VkRenderingAttachmentInfo colorAttachment = { .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO, .pNext = nullptr };

        colorAttachment.imageView = view;
        colorAttachment.imageLayout = layout;
        colorAttachment.loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        if (clear)
        {
            colorAttachment.clearValue = *clear;
        }

        return colorAttachment;
    }

    VkRenderingAttachmentInfo DepthAttachmentInfo(const VkImageView view, const VkImageLayout layout)
    {
        VkRenderingAttachmentInfo depthAttachment = { .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO, .pNext = nullptr };

        depthAttachment.imageView = view;
        depthAttachment.imageLayout = layout;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.clearValue.depthStencil.depth = 0.0f;

        return depthAttachment;
    }

    VkRenderingInfo RenderingInfo(const VkExtent2D renderExtent, const VkRenderingAttachmentInfo* colorAttachment, const VkRenderingAttachmentInfo* depthAttachment)
    {
        VkRenderingInfo renderInfo = { .sType = VK_STRUCTURE_TYPE_RENDERING_INFO, .pNext = nullptr };

        constexpr VkOffset2D offset = VkOffset2D { 0, 0 };
        renderInfo.renderArea = VkRect2D { offset, renderExtent };
        renderInfo.layerCount = 1;
        renderInfo.colorAttachmentCount = 1;
        renderInfo.pColorAttachments = colorAttachment;
        renderInfo.pDepthAttachment = depthAttachment;
        renderInfo.pStencilAttachment = nullptr;

        return renderInfo;
    }

    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(const VkShaderStageFlagBits stage, const VkShaderModule shaderModule, const char* entry)
    {
        VkPipelineShaderStageCreateInfo info = { .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, .pNext = nullptr };

        info.stage = stage;
        info.module = shaderModule; // Module containing the code for this shader stage.
        info.pName = entry; // Name of shader entry function.

        return info;
    }

    VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo()
    {
        VkPipelineLayoutCreateInfo info = {.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, .pNext = nullptr};

        info.flags = 0;
        info.setLayoutCount = 0;
        info.pSetLayouts = nullptr;
        info.pushConstantRangeCount = 0;
        info.pPushConstantRanges = nullptr;

        return info;
    }

    VkImageCreateInfo ImageCreateInfo(const VkFormat format, const VkImageUsageFlags usageFlags, const VkExtent3D extent)
    {
        VkImageCreateInfo info = { .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, .pNext = nullptr };

        info.imageType = VK_IMAGE_TYPE_2D;

        info.format = format;
        info.extent = extent;

        info.mipLevels = 1;
        info.arrayLayers = 1;

        // For MSAA. we will not be using it by default, so default it to 1 sample per pixel.
        info.samples = VK_SAMPLE_COUNT_1_BIT;

        // Optimal tiling, which means the image is stored on the best gpu format.
        info.tiling = VK_IMAGE_TILING_OPTIMAL;
        info.usage = usageFlags;

        return info;
    }

    VkImageViewCreateInfo ImageViewCreateInfo(const VkFormat format, const VkImage image, const VkImageAspectFlags aspectFlags)
    {
        // Build a image-view for the depth image to use for rendering.
        VkImageViewCreateInfo info = { .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, .pNext = nullptr };

        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.image = image;
        info.format = format;

        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        info.subresourceRange.aspectMask = aspectFlags;

        return info;
    }

    VkImageSubresourceRange ImageSubresourceRange(const VkImageAspectFlags aspectMask)
    {
        VkImageSubresourceRange subImage = { };

        subImage.aspectMask = aspectMask;
        subImage.baseMipLevel = 0;
        subImage.levelCount = VK_REMAINING_MIP_LEVELS;
        subImage.baseArrayLayer = 0;
        subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

        return subImage;
    }
}
