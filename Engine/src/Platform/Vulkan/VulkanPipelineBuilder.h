#pragma once
#include <vulkan/vulkan_core.h>
#include "vk_mem_alloc.h"

#include "VkBootstrap.h"

namespace Vulkan
{
    enum BlendMode
    {
        None,
        Additive,
        Alpha,
    };

    struct PipelineWrapper
    {
        VkPipeline Pipeline = VK_NULL_HANDLE;
        vkb::Device LogicalDevice;
        VkAllocationCallbacks* AllocationCallbacks = VK_NULL_HANDLE;

        // A conversion function which allows this Instance to be used
        // in places where VkPipeline would have been used.
        operator VkPipeline() const;
    };

    class VulkanPipelineBuilder
    {
    public:
        explicit VulkanPipelineBuilder(vkb::Device const& logicalDevice, VkPipelineLayout pipelineLayout);
        explicit VulkanPipelineBuilder(VkDevice const& logicalDevice, VkPipelineLayout pipelineLayout);

        vkb::Result<PipelineWrapper> Build() const;

        VulkanPipelineBuilder& SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);

        VulkanPipelineBuilder& SetTopology(VkPrimitiveTopology topology);

        VulkanPipelineBuilder& SetPolygonMode(VkPolygonMode mode);

        VulkanPipelineBuilder& SetCullMode(VkCullModeFlags mode);

        VulkanPipelineBuilder& SetWindingOrder(VkFrontFace order);

        VulkanPipelineBuilder& SetBlendMode(BlendMode mode);

        VulkanPipelineBuilder& SetAllocationCallbacks(VkAllocationCallbacks* callbacks);

        VulkanPipelineBuilder& SetColorFormat(VkFormat format);

        VulkanPipelineBuilder& SetDepthFormat(VkFormat format);

        VulkanPipelineBuilder& SetDepthTestOperator(VkCompareOp op);

        VulkanPipelineBuilder& SetDepthWrite(bool enabled);

    private:

        struct PipelineInfo
        {
            VkDevice LogicalDevice = VK_NULL_HANDLE;
            VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;

            std::vector<VkPipelineShaderStageCreateInfo> ShaderStageInfos;

            VkPrimitiveTopology Topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

            VkPolygonMode PolygonMode = VK_POLYGON_MODE_FILL;

            VkCullModeFlags CullMode = VK_CULL_MODE_NONE;
            VkFrontFace WindingOrder = VK_FRONT_FACE_CLOCKWISE;

            VkFormat ColorAttachmentFormat;
            VkFormat DepthAttachmentFormat;

            bool DepthTestEnabled = true;
            bool DepthWriteEnabled = true;
            VkCompareOp DepthTestOperator = VK_COMPARE_OP_LESS_OR_EQUAL;

            VkPipelineColorBlendAttachmentState ColorBlendAttachment;
            bool BlendEnabled = false;
            VkBlendFactor SrcColorFactor;
            VkBlendFactor DstColorFactor;
            VkBlendOp ColorBlendOp;
            VkBlendFactor SrcAlphaFactor;
            VkBlendFactor DstAlphaFactor;
            VkBlendOp AlphaBlendOp;

            VkAllocationCallbacks* AllocationCallbacks = VK_NULL_HANDLE;
        } _info;
    };
}
