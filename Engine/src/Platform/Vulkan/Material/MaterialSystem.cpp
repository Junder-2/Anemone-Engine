#include "anepch.h"
#include "MaterialSystem.h"

#include "Shader.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanUtils.h"

namespace Vulkan
{
    void MaterialSystem::Init(VulkanRenderer* owner)
    {
        _renderer = owner;
        BuildDefault();
    }

    void MaterialSystem::BuildDefault()
    {
        //TODO
    }

    void MaterialSystem::Cleanup()
    {

    }

    Shader* MaterialSystem::BuildShaderPass(const std::string& shaderName)
    {
        Shader* shader = new Shader();

        ShaderModule vertShader, fragShader;
        VulkanUtils::LoadSlangShaders(shaderName.c_str(), _renderer->GetDevice(), _renderer->GetAllocator(), &vertShader, &fragShader);

        shader->AddStage(vertShader, VK_SHADER_STAGE_VERTEX_BIT); //Is this redundant necessary?
        shader->AddStage(fragShader, VK_SHADER_STAGE_FRAGMENT_BIT);

        VulkanPipelineBuilder pipelineBuilder { _renderer->GetDevice(), shader->Layout };
        vkb::Result<PipelineWrapper> pipeline = pipelineBuilder
            .Build();

        shader->Pipeline = pipeline.value().Pipeline;

        return shader;
    }

    Material* MaterialSystem::BuildMaterial(const std::string& materialName, const MaterialData& info)
    {
        //TODO
        return nullptr;
    }

    Material* MaterialSystem::GetMaterial(const std::string& materialName)
    {
        if(!_materials.contains(materialName)) return nullptr;
        return _materials[materialName];
    }

    bool MaterialData::operator==(const MaterialData& other) const
    {
        if (other.baseTemplate != baseTemplate || other.parameters != parameters || other.textures.size() != textures.size()) return false;

        //binary compare textures
        bool comp = memcmp(other.textures.data(), textures.data(), textures.size() * sizeof(textures[0])) == 0;
        return comp;
    }

    size_t MaterialData::Hash() const
    {
        using std::size_t;
        using std::hash;

        size_t result = hash<std::string>()(baseTemplate);

        for (const auto& tex : textures)
        {
            //pack the binding data into a single int64. Not fully correct but its ok
            size_t textureHash = (std::hash<size_t>()((size_t)tex.Sampler) << 3) &&(std::hash<size_t>()((size_t)tex.View) >> 7);

            //shuffle the packed binding data and xor it with the main hash
            result ^= std::hash<size_t>()(textureHash);
        }

        return result;
    }
}

