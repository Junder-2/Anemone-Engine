#pragma once
#include <vulkan/vulkan_core.h>

#include "VmaTypes.h"
#include "VulkanDescriptors.h"
#include "ANE/Math/Types/Vector2.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Math/Types/Vector4.h"

namespace Vulkan
{
    using namespace Engine;
    class VulkanRenderer;
    struct MaterialInstance;

    enum class MaterialPass : uint8_t
    {
        Opaque,
        Transparent,
        Other,
    };

    struct ShaderPipeline
    {
        VkPipeline Pipeline;
        VkPipelineLayout Layout;
    };

    struct FilamentMetallicRoughness
    {
    public:
        struct MaterialConstants
        {
            Vector3 Color = 1; // = 1,1,1
            float Normal = 1; // = 1
            Vector3 Emission = 0; // = 0,0,0
            float Metallic = 1; // = 1
            float Roughness = 0; // = 0
            float Reflectance = 1; // = 1
            float Height = 1; // = 1
            float Occlusion = 1; // = 1

            // Padding so we meet 256 byte alignment.
            Vector4 Extra[13];
        };

        struct MaterialResources
        {
            VmaImage ColorImage;
            VkSampler ColorSampler;

            VmaImage NormalImage;
            VkSampler NormalSampler;

            // Occlusion, Roughness, Metallic.
            VmaImage ORMImage;
            VkSampler ORMSampler;

            VmaImage DFGImage;
            VkSampler DFGSampler;

            VmaImage CubeMap;
            VkSampler CubeMapSampler;

            VmaBuffer Data;
            //VkBuffer DataBuffer;
            uint32_t DataBufferOffset;
        };

    public:
        void BuildPipelines(const VulkanRenderer* renderer);

        void ClearResources(const VulkanRenderer* renderer);

        MaterialInstance WriteMaterial(const VulkanRenderer* renderer, MaterialPass type, const MaterialResources& resources, MaterialConstants* uniforms, DescriptorAllocator& descriptorAllocator);

    private:
        void LoadShaders(const VulkanRenderer* renderer, VkShaderModule& vertexShader, VkShaderModule& fragmentShader);

        void CreatePipelineLayout(const VulkanRenderer* renderer, VkPipelineLayout& pipelineLayout);

    public:
        ShaderPipeline Pipeline;
        VkDescriptorSetLayout MaterialLayout;

        DescriptorWriter Writer;
    };

    struct MaterialInstance
    {
        ShaderPipeline* ShaderPipeline;
        VkDescriptorSet MaterialSet;
        MaterialPass PassType;
        FilamentMetallicRoughness::MaterialConstants* Uniforms;
        FilamentMetallicRoughness::MaterialResources Resources;
    };
}
