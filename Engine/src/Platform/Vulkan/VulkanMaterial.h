#pragma once
#include <vulkan/vulkan_core.h>

#include "VmaTypes.h"
#include "VulkanDescriptors.h"
#include "ANE/Math/Types/Vector2.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Math/Types/Vector4.h"

namespace Engine
{
    class VulkanRenderer;

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

    struct MaterialInstance
    {
        ShaderPipeline* Pipeline;
        VkDescriptorSet MaterialSet;
        MaterialPass PassType;
    };

    struct FilamentMetallicRoughness
    {
    public:
        struct MaterialConstants
        {
            Vector3 Color;
            float Normal;
            Vector3 Emission;
            float Metallic;
            float Roughness;
            float Reflectance;
            float Height;
            float Occlusion;

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

            VkBuffer DataBuffer;
            uint32_t DataBufferOffset;
        };

    public:
        void BuildPipelines(const VulkanRenderer* renderer);

        void ClearResources(const VulkanRenderer* renderer);

        MaterialInstance WriteMaterial(const VulkanRenderer* renderer, MaterialPass type, const MaterialResources& resources, DescriptorAllocator& descriptorAllocator);

    private:
        void LoadShaders(const VulkanRenderer* renderer, VkShaderModule& vertexShader, VkShaderModule& fragmentShader);

        void CreatePipelineLayout(const VulkanRenderer* renderer, VkPipelineLayout& pipelineLayout);

    public:
        ShaderPipeline Pipeline;
        VkDescriptorSetLayout MaterialLayout;

        DescriptorWriter Writer;
    };
}
