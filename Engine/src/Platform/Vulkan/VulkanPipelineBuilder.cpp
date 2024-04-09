#include "anepch.h"
#include "VulkanPipelineBuilder.h"

#include "VulkanInitializers.h"

namespace Engine
{
    VulkanPipelineBuilder::VulkanPipelineBuilder(vkb::Device const& logicalDevice, const VkPipelineLayout pipelineLayout)
    {
        _info.LogicalDevice = logicalDevice.device;
        _info.PipelineLayout = pipelineLayout;
    }

    vkb::Result<PipelineWrapper> VulkanPipelineBuilder::Build() const
    {
        VkPipelineViewportStateCreateInfo viewportStateInfo = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr };
        viewportStateInfo.viewportCount = 1;
        viewportStateInfo.scissorCount = 1;

        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = _info.BlendEnabled ? VK_TRUE : VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = _info.SrcColorFactor;
        colorBlendAttachment.dstColorBlendFactor = _info.DstColorFactor;
        colorBlendAttachment.colorBlendOp = _info.ColorBlendOp;
        colorBlendAttachment.srcAlphaBlendFactor = _info.SrcAlphaFactor;
        colorBlendAttachment.dstAlphaBlendFactor = _info.DstAlphaFactor;
        colorBlendAttachment.alphaBlendOp = _info.AlphaBlendOp;

        VkPipelineColorBlendStateCreateInfo colorBlendingInfo = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, nullptr };
        colorBlendingInfo.logicOpEnable = VK_FALSE;
        colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendingInfo.attachmentCount = 1;
        colorBlendingInfo.pAttachments = &colorBlendAttachment;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
        inputAssemblyInfo.topology = _info.Topology;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        VkPipelineRasterizationStateCreateInfo rasterizationInfo = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
        rasterizationInfo.lineWidth = 1.0f;
        rasterizationInfo.polygonMode = _info.PolygonMode;

        VkPipelineMultisampleStateCreateInfo multisampleInfo = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo.minSampleShading = 1.0f;
        // Alpha coverage
        multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleInfo.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencilInfo = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
        depthStencilInfo.depthTestEnable = _info.DepthTestEnabled ? VK_TRUE : VK_FALSE;;
        depthStencilInfo.depthWriteEnable = _info.DepthWriteEnabled ? VK_TRUE : VK_FALSE;;
        depthStencilInfo.depthCompareOp = _info.DepthTestOperator;
        // Bounds
        depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilInfo.minDepthBounds = 0.0f;
        depthStencilInfo.maxDepthBounds = 1.0f;
        // Stencil
        depthStencilInfo.stencilTestEnable = VK_FALSE;
        depthStencilInfo.front = { };
        depthStencilInfo.back = { };

        VkPipelineRenderingCreateInfo renderingInfo = { VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachmentFormats = &_info.ColorAttachmentFormat;
        renderingInfo.depthAttachmentFormat = _info.DepthAttachmentFormat;

        VkGraphicsPipelineCreateInfo pipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
        pipelineInfo.pNext = &renderingInfo;

        pipelineInfo.stageCount = (uint32_t)_info.ShaderStageInfos.size();
        pipelineInfo.pStages = _info.ShaderStageInfos.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportStateInfo;
        pipelineInfo.pRasterizationState = &rasterizationInfo;
        pipelineInfo.pMultisampleState = &multisampleInfo;
        pipelineInfo.pDepthStencilState = &depthStencilInfo;
        pipelineInfo.pColorBlendState = &colorBlendingInfo;
        pipelineInfo.layout = _info.PipelineLayout;

        VkDynamicState state[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

        VkPipelineDynamicStateCreateInfo dynamicInfo = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
        dynamicInfo.pDynamicStates = &state[0];
        dynamicInfo.dynamicStateCount = 2;

        pipelineInfo.pDynamicState = &dynamicInfo;

        PipelineWrapper newPipeline;
        VkResult result = vkCreateGraphicsPipelines(_info.LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, _info.AllocationCallbacks, &newPipeline.Pipeline);
        if (result != VK_SUCCESS)
        {
            return vkb::Result<PipelineWrapper>(vkb::InstanceError::failed_create_debug_messenger, result); // Failed to create graphics pipeline.
        }

        return newPipeline;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetShaders(const VkShaderModule vertexShader, const VkShaderModule fragmentShader)
    {
        _info.ShaderStageInfos.clear();
        _info.ShaderStageInfos.push_back(VulkanInitializers::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));
        _info.ShaderStageInfos.push_back(VulkanInitializers::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetTopology(const VkPrimitiveTopology topology)
    {
        _info.Topology = topology;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetPolygonMode(const VkPolygonMode mode)
    {
        _info.PolygonMode = mode;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetCullMode(const VkCullModeFlags mode)
    {
        _info.CullMode = mode;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetWindingOrder(const VkFrontFace order)
    {
        _info.WindingOrder = order;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetBlendMode(const BlendMode mode)
    {
        switch (mode)
        {
            case None: // TODO: Figure out if we need to set factors/operators for no blend.
                _info.BlendEnabled = false;
                break;
            case Additive:
                _info.BlendEnabled = true;
                _info.SrcColorFactor = VK_BLEND_FACTOR_ONE;
                _info.DstColorFactor = VK_BLEND_FACTOR_DST_ALPHA;
                _info.ColorBlendOp = VK_BLEND_OP_ADD;
                _info.SrcAlphaFactor = VK_BLEND_FACTOR_ONE;
                _info.DstAlphaFactor = VK_BLEND_FACTOR_ZERO;
                _info.AlphaBlendOp = VK_BLEND_OP_ADD;
                break;
            case Alpha:
                _info.BlendEnabled = true;
                _info.SrcColorFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
                _info.DstColorFactor = VK_BLEND_FACTOR_DST_ALPHA;
                _info.ColorBlendOp = VK_BLEND_OP_ADD;
                _info.SrcAlphaFactor = VK_BLEND_FACTOR_ONE;
                _info.DstAlphaFactor = VK_BLEND_FACTOR_ZERO;
                _info.AlphaBlendOp = VK_BLEND_OP_ADD;
            default: break;
        }

        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetAllocationCallbacks(VkAllocationCallbacks* callbacks)
    {
        _info.AllocationCallbacks = callbacks;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetColorFormat(const VkFormat format)
    {
        _info.ColorAttachmentFormat = format;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetDepthFormat(const VkFormat format)
    {
        _info.DepthAttachmentFormat = format;
        return *this;
    }

    // TODO: Determine behaviour if disabling depth testing is necessary.
    VulkanPipelineBuilder& VulkanPipelineBuilder::SetDepthTestOperator(const VkCompareOp op)
    {
        _info.DepthTestEnabled = op != VK_COMPARE_OP_ALWAYS;
        _info.DepthTestOperator = op;
        return *this;
    }

    VulkanPipelineBuilder& VulkanPipelineBuilder::SetDepthWrite(const bool enabled)
    {
        _info.DepthWriteEnabled = enabled;
        return *this;
    }
}
