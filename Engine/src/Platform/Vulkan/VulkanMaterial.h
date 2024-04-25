#pragma once
#include <vulkan/vulkan_core.h>

namespace Engine
{
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

    struct BRDFMaterial
    {

    };
}
