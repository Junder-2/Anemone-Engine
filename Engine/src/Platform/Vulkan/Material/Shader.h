#pragma once
#include "Platform/Vulkan/VmaTypes.h"

namespace Vulkan
{
    struct ReflectionOverrides
    {
        const char* Name;
        VkDescriptorType OverridenType;
    };

    struct ShaderStage
    {
        VkShaderModule* ShaderModule;
        VkShaderStageFlagBits Stage;
    };

    struct ReflectedBinding {
        uint32_t Set;
        uint32_t Bindings;
        VkDescriptorType Type;
    };

    class Shader
    {
    public:
        void AddStage(VkShaderModule* shaderModule, VkShaderStageFlagBits stage);

        void ReflectLayout(VkDevice device, ReflectionOverrides* overrides, int overrideCount);

        void FillStages(std::vector<VkPipelineShaderStageCreateInfo>& pipelineStages);

        VkPipeline Pipeline;
        VkPipelineLayout Layout;

    private:
        std::unordered_map<std::string, ReflectedBinding> _bindings;
        std::array<VkDescriptorSetLayout, 4> _setLayouts;
        std::array<uint32_t, 4> _setHashes;

        std::vector<ShaderStage> _stages;
    };
}
