﻿#pragma once
#include "Component.h"
#include "ANE/Math/Types/TransformMatrix.h"

namespace Engine
{
    struct TransformComponent : std::enable_shared_from_this<TransformComponent>, Component
    {
    public:
        TransformMatrix Transform {};

        std::shared_ptr<TransformComponent> Parent;
        std::vector<std::shared_ptr<TransformComponent>> Children;

        ANE_COMPONENT_INIT(TransformComponent)

        void SetParent(const TransformComponent& parent)
        {
            Parent = std::make_shared<TransformComponent>(parent);
            Parent->AddChild(*this);
        }

        void AddChild(const TransformComponent& child)
        {
            Children.push_back(std::make_shared<TransformComponent>(child));
        }

        void OnValidate()
        {

        }

        TransformComponent(const TransformMatrix& transform, const std::shared_ptr<TransformComponent>& parent = nullptr) : Component(typeid(*this).name()), Transform(transform), Parent(parent)
        {
            ANE_ELOG("We are not getting in here");
        }

        static void RegisterComponentMetaData()
        {
            entt::meta<TransformComponent>()
                .data<&TransformComponent::Transform>("Transform"_hs).prop("display_name"_hs, "Transform")
                .prop("Position"_hs, "Position")
                .prop("Rotation"_hs, "Rotation")
                .prop("Scale"_hs, "Scale")
                .EDITABLE
                .func<&TransformComponent::OnValidate>("OnValidate"_hs);
        }


        // lets you access the Transform, this is a glm::mat4 with extra fuss
         operator const TransformMatrix&() const { return Transform; }
         operator TransformMatrix&() { return Transform; }
    };

}
