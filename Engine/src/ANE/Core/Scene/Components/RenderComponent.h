#pragma once
#include <filesystem>

#include "Component.h"
#include "ANE/Renderer/Mesh.h"
#include "ANE/Renderer/Renderer.h"

//todo: finish, this is just a demo
namespace Engine
{
    struct MaterialData
    {
        Vulkan::MaterialInstance* Material = nullptr;
        bool HasUniqueMaterial = false;
    };

    struct RenderComponent : Component
    {
        //copy constructor
        ANE_COMPONENT_INIT(RenderComponent)

        RenderComponent(std::string modelPath) : Component(typeid(*this).name()), _modelPath(std::move(modelPath))
        {
            Model = Renderer::LoadModel(_modelPath);
            SetMaterial(Renderer::GetDefaultMaterial());
        }


        void VerifyDefaultMaterialBlock()
        {

        }

        void OnValidate()
        {
            VerifyDefaultMaterialBlock();
        }

        static void RegisterComponentMetaData()
        {
            entt::meta<RenderComponent>()
                //.data<&RenderComponent::_modelPath>("Model File Location"_hs).prop("display_name"_hs, "Model File Location")
                .data<&RenderComponent::Model>("Mesh Asset"_hs).prop("display_name"_hs, "Mesh Asset")
                .EDITABLE
                .data<&RenderComponent::_materialData>("Material Properties"_hs).prop("display_name"_hs, "Material Properties")
                .EDITABLE
                .func<&RenderComponent::OnValidate>("OnValidate"_hs);
        }

        std::string GetModelPath()
        {
            return _modelPath;
        }

        Vulkan::MaterialInstance* GetMaterial() const { return _materialData.Material; }
        void SetMaterial(Vulkan::MaterialInstance* material)
        {
            _materialData.Material = material;
        }

    public:
        VmaMeshAsset Model = {};

    private:
        MaterialData _materialData;
        std::string _modelPath;
    };
}
