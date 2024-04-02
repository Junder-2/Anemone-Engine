#pragma once
#include "../../TransformMatrix.h"

namespace Engine
{
    struct TransformComponent : std::enable_shared_from_this<TransformComponent>
    {
    public:
        TransformMatrix Transform;
        const char* Name;

        std::shared_ptr<TransformComponent> Parent;
        std::vector<std::shared_ptr<TransformComponent>> Children;

        ~TransformComponent() = default;
        //copy constructor
        TransformComponent() = default;

        void SetParent(const TransformComponent& parent)
        {
            Parent = std::make_shared<TransformComponent>(parent);
            Parent->AddChild(*this);
        }

        void AddChild(const TransformComponent& child)
        {
            Children.push_back(std::make_shared<TransformComponent>(child));
        }

        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const TransformMatrix& transform, const char* name = "Undefined", const std::shared_ptr<TransformComponent>& parent = nullptr) : Transform(transform), Name(name), Parent(parent) { };


        // lets you access the Transform, this is a glm::mat4 with extra fuss
         operator const TransformMatrix&() const { return Transform; }
         operator TransformMatrix&() { return Transform; }
    };
}
