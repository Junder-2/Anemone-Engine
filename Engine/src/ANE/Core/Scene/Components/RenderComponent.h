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

    public:
        RenderComponent(std::string modelPath) : Component(typeid(*this).name()), _modelPath(std::move(modelPath))
        {
            Model = Renderer::LoadModel(_modelPath);
        }


        VmaMeshAsset Model = {};

    private:
        std::string _modelPath;
    };

}
