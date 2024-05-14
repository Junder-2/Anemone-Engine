#include "anepch.h"
#include "Shader.h"

#include "Platform/Vulkan/VulkanInitializers.h"

namespace Vulkan
{
    void Shader::AddStage(VkShaderModule* shaderModule, const VkShaderStageFlagBits stage)
    {
        const ShaderStage newStage = {shaderModule, stage};
        _stages.push_back(newStage);
    }

    struct DescriptorSetLayoutData
    {
        uint32_t SetNumber;
        VkDescriptorSetLayoutCreateInfo CreateInfo;
        std::vector<VkDescriptorSetLayoutBinding> Bindings;
    };

    void Shader::ReflectLayout(VkDevice device, ReflectionOverrides* overrides, int overrideCount)
    {
        std::vector<DescriptorSetLayoutData> setLayouts;

        std::vector<VkPushConstantRange> constantRanges;

        //TODO need to convert spv reflection to slang

        //we start from just the default empty pipeline layout info
        VkPipelineLayoutCreateInfo meshPipelineLayoutInfo = VulkanInitializers::PipelineLayoutCreateInfo();

        meshPipelineLayoutInfo.pPushConstantRanges = constantRanges.data();
        meshPipelineLayoutInfo.pushConstantRangeCount = (uint32_t)constantRanges.size();

        std::array<VkDescriptorSetLayout,4> compactedLayouts;
        int s = 0;
        for (int i = 0; i < 4; i++)
        {
            if (_setLayouts[i] != VK_NULL_HANDLE)
            {
                compactedLayouts[s] = _setLayouts[i];
                s++;
            }
        }

        meshPipelineLayoutInfo.setLayoutCount = s;
        meshPipelineLayoutInfo.pSetLayouts = compactedLayouts.data();


        vkCreatePipelineLayout(device, &meshPipelineLayoutInfo, nullptr, &Layout);
	}

    void Shader::FillStages(std::vector<VkPipelineShaderStageCreateInfo>& pipelineStages)
    {
        for (const auto& stage : _stages)
        {
            pipelineStages.push_back(VulkanInitializers::PipelineShaderStageCreateInfo(stage.Stage, *stage.ShaderModule));
        }
    }
}
