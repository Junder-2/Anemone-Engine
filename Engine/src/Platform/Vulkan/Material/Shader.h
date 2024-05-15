#pragma once
#include <entt.hpp>
#include <slang-com-ptr.h>

using namespace Slang;

#include "Platform/Vulkan/VmaTypes.h"

namespace Vulkan
{
    class VulkanRenderer;

    struct ReflectionOverrides
    {
        const char* Name;
        VkDescriptorType OverridenType;
    };

    struct ReflectedBinding
    {
        uint32_t Set;
        uint32_t Bindings;
        VkDescriptorType Type;
    };

    struct ShaderParameters
    {
        VkPushConstantRange PushConstant;
        std::vector<ReflectedBinding> Bindings;
    };

    struct ShaderModule
    {
        VkShaderModule Module;
        ShaderParameters Parameters;
    };

    struct ShaderStage
    {
        ShaderModule ShaderModule;
        VkShaderStageFlagBits Stage;
    };

    class Shader
    {
    public:
        void AddStage(const ShaderModule& shaderModule, VkShaderStageFlagBits stage);

        void ReflectLayout(VulkanRenderer* renderer, ReflectionOverrides* overrides, int overrideCount);

        void FillStages(std::vector<VkPipelineShaderStageCreateInfo>& pipelineStages);

        VkPipeline Pipeline;
        VkPipelineLayout Layout;

    private:
        std::unordered_map<std::string, ReflectedBinding> _bindings;
        std::array<VkDescriptorSetLayout, 4> _setLayouts;
        std::array<uint32_t, 4> _setHashes;

        std::vector<ShaderStage> _stages;
    };

    class ShaderCache
    {
        void Init(VulkanRenderer* owner) { _renderer = owner; }

        Shader* GetShader(const std::string& shaderName);

    private:
        void LoadShaderModules(const VulkanRenderer* renderer, const char* shaderName, ShaderModule& vertModule, ShaderModule& fragModule);
        ComPtr<slang::IComponentType> LoadSlangEntry(const ComPtr<slang::ISession>& session, slang::IModule* slangModule, char const* entryName);
        ShaderParameters GetShaderParameters(const ComPtr<slang::IComponentType>& program);
        ComPtr<slang::IBlob> LoadSpirVProgram(const ComPtr<slang::IComponentType>& slangEntry);
        ShaderModule LoadShaderModule(const VulkanRenderer* renderer, const ComPtr<slang::ISession>& session, slang::IModule* slangModule, char const* entryName);

    private:
        VulkanRenderer* _renderer = {};
        entt::dense_map<std::string, Shader> _shaderMap;
    };
}
