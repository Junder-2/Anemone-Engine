#include "anepch.h"
#include "Shader.h"

#include "Platform/Vulkan/VulkanInitializers.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanUtils.h"

namespace Vulkan
{
    void Shader::AddStage(const ShaderModule& shaderModule, const VkShaderStageFlagBits stage)
    {
        const ShaderStage newStage = { shaderModule, stage };
        _stages.push_back(newStage);
    }

    struct DescriptorSetLayoutData
    {
        uint32_t SetNumber;
        VkDescriptorSetLayoutCreateInfo CreateInfo;
        std::vector<VkDescriptorSetLayoutBinding> Bindings;
    };

    void Shader::ReflectLayout(VulkanRenderer* renderer, ReflectionOverrides* overrides, int overrideCount)
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

        vkCreatePipelineLayout(renderer->GetDevice(), &meshPipelineLayoutInfo, renderer->GetAllocator(), &Layout);
    }

    void Shader::FillStages(std::vector<VkPipelineShaderStageCreateInfo>& pipelineStages)
    {
        for (const auto& stage : _stages)
        {
            pipelineStages.push_back(VulkanInitializers::PipelineShaderStageCreateInfo(stage.Stage, stage.ShaderModule.Module));
        }
    }

    Shader* ShaderCache::GetShader(const std::string& shaderName)
    {
        if (_shaderMap.contains(shaderName))
        {
            return &_shaderMap[shaderName];
        }

        ShaderModule vertModule, fragModule;
        LoadShaderModules(_renderer, shaderName.c_str(), vertModule, fragModule);
        VulkanUtils::LoadSlangShaders(shaderName.c_str(), _renderer->GetDevice(), _renderer->GetAllocator(), &vertModule, &fragModule);

        Shader shader = {};
        shader.AddStage(vertModule, VK_SHADER_STAGE_VERTEX_BIT);
        shader.AddStage(fragModule, VK_SHADER_STAGE_FRAGMENT_BIT);
        //shader.ReflectLayout(_renderer, );

        VulkanPipelineBuilder pipelineBuilder { _renderer->GetDevice(), shader.Layout };
        vkb::Result<PipelineWrapper> pipeline = pipelineBuilder
            .SetShaders(vertModule.Module, fragModule.Module)
            .Build();

        shader.Pipeline = pipeline.value();

        _shaderMap[shaderName] = shader;
        return &_shaderMap[shaderName];
    }

    void ShaderCache::LoadShaderModules(const VulkanRenderer* renderer, const char* shaderName, ShaderModule& vertModule, ShaderModule& fragModule)
    {
        ComPtr<slang::IGlobalSession> slangGlobalSession;
        createGlobalSession(slangGlobalSession.writeRef());

        slang::TargetDesc targetDesc = { };
        targetDesc.format = SLANG_SPIRV;
        targetDesc.profile = slangGlobalSession->findProfile("glsl_440");
        targetDesc.flags = SLANG_TARGET_FLAG_GENERATE_SPIRV_DIRECTLY;

        slang::SessionDesc sessionDesc = { };
        sessionDesc.targets = &targetDesc;
        sessionDesc.targetCount = 1;
        sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;
        const char* paths[] = { ASSET_PATH_SHADERS };
        sessionDesc.searchPaths = paths;
        sessionDesc.searchPathCount = 1;

        ComPtr<slang::ISession> session;
        slangGlobalSession->createSession(sessionDesc, session.writeRef());

        ComPtr<slang::IBlob> diagnosticBlob;
        slang::IModule* slangModule = session->loadModule(shaderName, diagnosticBlob.writeRef());
        if (!slangModule)
        {
            ANE_ELOG_ERROR("Error when loading Slang module: {}", shaderName);
        }

        // TODO: Validation using diagnosticBlob.

        vertModule = LoadShaderModule(renderer, session, slangModule, "vertexMain");
        fragModule = LoadShaderModule(renderer, session, slangModule, "fragmentMain");
    }

    ComPtr<slang::IComponentType> ShaderCache::LoadSlangEntry(const ComPtr<slang::ISession>& session, slang::IModule* slangModule, char const* entryName)
    {
        ComPtr<slang::IEntryPoint> entry;
        slangModule->findEntryPointByName(entryName, entry.writeRef());

        slang::IComponentType* components[] = { slangModule, entry };
        ComPtr<slang::IComponentType> program;
        ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = session->createCompositeComponentType(
            components,
            2,
            program.writeRef(),
            diagnosticsBlob.writeRef());

        // TODO: Validation.

        return program;
    }

    ShaderParameters ShaderCache::GetShaderParameters(const ComPtr<slang::IComponentType>& program)
    {
        ShaderParameters params = {};

        slang::ShaderReflection* reflection = program->getLayout();
        const unsigned paramCount = reflection->getParameterCount();
        for (unsigned paramIndex = 0; paramIndex < paramCount; paramIndex++)
        {
            slang::VariableLayoutReflection* parameter = reflection->getParameterByIndex(paramIndex);
            slang::TypeLayoutReflection* typeLayout = parameter->getTypeLayout();

            if (typeLayout->getKind() == slang::TypeReflection::Kind::ConstantBuffer)
            {

            }
        }

        return params;
    }

    ComPtr<slang::IBlob> ShaderCache::LoadSpirVProgram(const ComPtr<slang::IComponentType>& slangEntry)
    {
        ComPtr<slang::IBlob> spirVProgram;
        ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = slangEntry->getEntryPointCode(0, 0, spirVProgram.writeRef(), diagnosticsBlob.writeRef());

        // TODO: Validation.

        return spirVProgram;
    }

    ShaderModule ShaderCache::LoadShaderModule(const VulkanRenderer* renderer, const ComPtr<slang::ISession>& session, slang::IModule* slangModule, char const* entryName)
    {
        ShaderModule module;
        const ComPtr<slang::IComponentType> slangProgram = LoadSlangEntry(session, slangModule, entryName);
        module.Parameters = GetShaderParameters(slangProgram);

        const ComPtr<slang::IBlob> spirVFragProgram = LoadSpirVProgram(slangProgram);
        if (!VulkanUtils::LoadShaderModule(spirVFragProgram, renderer->GetDevice(), renderer->GetAllocator(), &module.Module))
        {
            ANE_ELOG_ERROR("Error when building shader module: {}", slangModule->getName());
        }

        return module;
    }
}
