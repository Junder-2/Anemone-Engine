#pragma once
#include <filesystem>

#include "Component.h"
#include "ANE/Renderer/Mesh.h"
#include "ANE/Renderer/Renderer.h"

//todo: finish, this is just a demo
namespace Engine
{
    struct RenderComponent : Component
    {
        //copy constructor
        ANE_COMPONENT_INIT(RenderComponent)

        RenderComponent(std::string modelPath) : Component(typeid(*this).name()), _modelPath(std::move(modelPath))
        {
            _materialPropertyBlock.Color = Vector3::OneVector();
            _materialPropertyBlock.Emission = Vector3::ZeroVector();
            _materialPropertyBlock.Metallic = 1;
            _materialPropertyBlock.Normal = 1;
            _materialPropertyBlock.Height = 1;
            _materialPropertyBlock.Occlusion = 1;
            _materialPropertyBlock.Reflectance = 1;
            _materialPropertyBlock.Roughness = 0;

            Model = Renderer::LoadModel(_modelPath);
        }

        VmaMeshAsset Model = {};
        void VerifyDefaultMaterialBlock()
        {
            //Could write a "compare to default values" check here and reassign to true
            _defaultMaterialValues = false;

        }

        void OnValidate()
        {
            VerifyDefaultMaterialBlock();
        }
        static void RegisterComponentMetaData()
        {
            entt::meta<RenderComponent>()
                .data<&RenderComponent::_modelPath>("Model File Location"_hs).prop("display_name"_hs, "Model File Location")
                .data<&RenderComponent::Model>("Mesh Asset"_hs).prop("display_name"_hs, "Mesh Asset")
                .EDITABLE
                .data<&RenderComponent::_materialPropertyBlock>("Material Properties"_hs).prop("display_name"_hs,"Material Properties Block")
                .EDITABLE
                .func<&RenderComponent::OnValidate>("OnValidate"_hs);
;
        }
    private:
        FilamentMetallicRoughness::MaterialConstants _materialPropertyBlock;
        bool _defaultMaterialValues = true;
        std::string _modelPath;
    };
}
