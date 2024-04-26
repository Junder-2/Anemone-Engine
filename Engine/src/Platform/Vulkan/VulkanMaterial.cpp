#include "anepch.h"
#include "VulkanMaterial.h"

#include <slang.h>
#include <slang-com-ptr.h>

using Slang::ComPtr;

#include "VulkanCommon.h"
#include "VulkanDescriptorLayoutBuilder.h"
#include "VulkanRenderer.h"
#include "VulkanInitializers.h"
#include "VulkanUtils.h"

namespace Engine
{
    void FilamentMetallicRoughness::BuildPipelines(const VulkanRenderer* renderer)
    {
        VkShaderModule vertShader, fragShader;
        LoadShaders(renderer, vertShader, fragShader);

        CreatePipelineLayout(renderer, Pipeline.Layout);

        const VkDevice device = renderer->GetDevice();
        const VkAllocationCallbacks* allocator = renderer->GetAllocator();

        VulkanPipelineBuilder pipelineBuilder { device, Pipeline.Layout };
        vkb::Result<PipelineWrapper> pipeline = pipelineBuilder
            .SetShaders(vertShader, fragShader)
            .SetBlendMode(None)
            .SetDepthTestOperator(VK_COMPARE_OP_GREATER_OR_EQUAL)

            .SetColorFormat(renderer->GetColorBuffer().ImageFormat)
            .SetDepthFormat(renderer->GetDepthBuffer().ImageFormat)

            .Build();

        Pipeline.Pipeline = pipeline.value().Pipeline;

        vkDestroyShaderModule(device, vertShader, allocator);
        vkDestroyShaderModule(device, fragShader, allocator);
    }

    void FilamentMetallicRoughness::ClearResources(const VulkanRenderer* renderer)
    {
        const VkDevice device = renderer->GetDevice();
        const VkAllocationCallbacks* allocator = renderer->GetAllocator();

        vkDestroyDescriptorSetLayout(device, MaterialLayout, allocator);
        vkDestroyPipelineLayout(device, Pipeline.Layout, allocator);
        vkDestroyPipeline(device, Pipeline.Pipeline, allocator);
    }

    MaterialInstance FilamentMetallicRoughness::WriteMaterial(const VulkanRenderer* renderer, const MaterialPass type, const MaterialResources& resources, DescriptorAllocator& descriptorAllocator)
    {
        const VkDevice device = renderer->GetDevice();
        const VkAllocationCallbacks* allocator = renderer->GetAllocator();

        MaterialInstance materialData;
        materialData.Pipeline = &Pipeline;
        materialData.MaterialSet = descriptorAllocator.Allocate(device, MaterialLayout, allocator);
        materialData.PassType = type;

        constexpr VkImageLayout readOnly = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        constexpr VkDescriptorType combinedSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        Writer.Clear();
        Writer.WriteBuffer(0, resources.DataBuffer, sizeof(MaterialConstants), resources.DataBufferOffset, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        Writer.WriteImage(1, resources.ColorImage.ImageView, resources.ColorSampler, readOnly, combinedSampler);
        Writer.WriteImage(2, resources.NormalImage.ImageView, resources.NormalSampler, readOnly, combinedSampler);
        Writer.WriteImage(3, resources.ORMImage.ImageView, resources.ORMSampler, readOnly, combinedSampler);

        Writer.UpdateSet(device, materialData.MaterialSet);

        return materialData;
    }

    void FilamentMetallicRoughness::LoadShaders(const VulkanRenderer* renderer, VkShaderModule& vertexShader, VkShaderModule& fragmentShader)
    {
        // TODO: Add utility class to load Slang shaders from a path.
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
        const char* paths[] = { "../Assets/Shaders/" };
        sessionDesc.searchPaths = paths;
        sessionDesc.searchPathCount = 1;

        ComPtr<slang::ISession> session;
        slangGlobalSession->createSession(sessionDesc, session.writeRef());

        ComPtr<slang::IBlob> spirVVertProgram;
        ComPtr<slang::IBlob> spirVFragProgram;
        {
            const char* moduleName = "Filament_Standard";
            ComPtr<slang::IBlob> diagnosticBlob;
            slang::IModule* slangModule = session->loadModule(moduleName, diagnosticBlob.writeRef());
            //diagnoseIfNeeded(diagnosticBlob);

            if (!slangModule)
            {
                ANE_ELOG_ERROR("Error when loading shader module: {}", moduleName);
            }

            ComPtr<slang::IEntryPoint> vertEntry;
            slangModule->findEntryPointByName("vertexMain", vertEntry.writeRef());

            slang::IComponentType* components[] = { slangModule, vertEntry };
            ComPtr<slang::IComponentType> program;
            {
                ComPtr<slang::IBlob> diagnosticsBlob;
                SlangResult result = session->createCompositeComponentType(
                    components,
                    2,
                    program.writeRef(),
                    diagnosticsBlob.writeRef());
                // diagnoseIfNeeded(diagnosticsBlob);
                // RETURN_ON_FAIL(result);
            }

            {
                ComPtr<slang::IBlob> diagnosticsBlob;
                SlangResult result = program->getEntryPointCode(
                    0, 0, spirVVertProgram.writeRef(), diagnosticsBlob.writeRef());
                //diagnoseIfNeeded(diagnosticsBlob);
                //RETURN_ON_FAIL(result);
            }

            ComPtr<slang::IEntryPoint> fragEntry;
            slangModule->findEntryPointByName("fragmentMain", fragEntry.writeRef());

            slang::IComponentType* components2[] = { slangModule, fragEntry };
            ComPtr<slang::IComponentType> program2;
            {
                ComPtr<slang::IBlob> diagnosticsBlob;
                SlangResult result = session->createCompositeComponentType(
                    components2,
                    2,
                    program2.writeRef(),
                    diagnosticsBlob.writeRef());
                // diagnoseIfNeeded(diagnosticsBlob);
                // RETURN_ON_FAIL(result);
            }

            {
                ComPtr<slang::IBlob> diagnosticsBlob;
                SlangResult result = program2->getEntryPointCode(
                    0, 0, spirVFragProgram.writeRef(), diagnosticsBlob.writeRef());
                //diagnoseIfNeeded(diagnosticsBlob);
                //RETURN_ON_FAIL(result);
            }
        }

        const VkDevice device = renderer->GetDevice();
        const VkAllocationCallbacks* allocator = renderer->GetAllocator();

        if (!VulkanUtils::LoadShaderModule(spirVVertProgram, device, allocator, &vertexShader))
        {
            ANE_ELOG_ERROR("Error when building vertex shader module: Filament-Standard.slang");
        }

        if (!VulkanUtils::LoadShaderModule(spirVFragProgram, device, allocator, &fragmentShader))
        {
            ANE_ELOG_ERROR("Error when building fragment shader module: Filament-Standard.slang");
        }
    }

    void FilamentMetallicRoughness::CreatePipelineLayout(const VulkanRenderer* renderer, VkPipelineLayout& pipelineLayout)
    {
        const VkDevice device = renderer->GetDevice();
        const VkAllocationCallbacks* allocator = renderer->GetAllocator();

        VkPushConstantRange matrixRange = { };
        matrixRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        matrixRange.offset = 0;
        matrixRange.size = sizeof(PushConstantBuffer);

        // TODO: Figure out how to handle buffer and texture sharing binding 0.
        DescriptorLayoutBuilder layoutBuilder{ device };
        MaterialLayout = layoutBuilder
            .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
            .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            .AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) // Color
            .AddBinding(2, VK_DESCRIPTOR_TYPE_SAMPLER)
            .AddBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) // Normal
            .AddBinding(4, VK_DESCRIPTOR_TYPE_SAMPLER)
            .AddBinding(5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) // Occlusion, Roughness, Metallic
            .AddBinding(6, VK_DESCRIPTOR_TYPE_SAMPLER)

            .Build();

        std::vector<VkDescriptorSetLayout> layouts = renderer->GetSceneLayouts();
        layouts.push_back(MaterialLayout);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = VulkanInitializers::PipelineLayoutCreateInfo();
        pipelineLayoutInfo.setLayoutCount = (uint32_t)layouts.size();
        pipelineLayoutInfo.pSetLayouts = layouts.data();
        pipelineLayoutInfo.pPushConstantRanges = &matrixRange;
        pipelineLayoutInfo.pushConstantRangeCount = 1;

        CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, allocator, &pipelineLayout));
    }
}
