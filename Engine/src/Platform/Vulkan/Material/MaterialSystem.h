#pragma once
#include <entt.hpp>

#include "Platform/Vulkan/VulkanPipelineBuilder.h"

// https://github.com/vblanco20-1/vulkan-guide/blob/engine/extra-engine/material_system.h
namespace Vulkan
{
    class Shader;
    class VulkanRenderer;

    struct SampledTexture
    {
        VkSampler Sampler;
        VkImageView View;
    };

    struct ShaderParameters;

    struct MaterialData {
        std::vector<SampledTexture> textures;
        ShaderParameters* parameters;
        std::string baseTemplate;

        bool operator==(const MaterialData& other) const;

        size_t Hash() const;
    };

    struct Material
    {
        Shader* Original;
        VkDescriptorSet Set; //Base system uses PerPassData<VkDescriptorSet> passSets;

        std::vector<SampledTexture> Textures;
        ShaderParameters* Parameters;
        Material& operator=(const Material& other) = default;
    };

    class MaterialSystem
    {
    public:
        void Init(VulkanRenderer* owner);
        void Cleanup();

        Shader* BuildShaderPass(const std::string& shaderName);

        Material* BuildMaterial(const std::string& materialName, const MaterialData& info);
        Material* GetMaterial(const std::string& materialName);

    private:
        void BuildDefault();

        struct MaterialInfoHash
        {
            std::size_t operator()(const MaterialData& k) const
            {
                return k.Hash();
            }
        };

    private:
        entt::dense_map<std::string, Material*> _materials;
        entt::dense_map<MaterialData, Material*, MaterialInfoHash> _materialCache;

        VulkanRenderer* _renderer {};
    };
}
