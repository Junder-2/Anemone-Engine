﻿#pragma once
#include "Component.h"
#include "../../TransformMatrix.h"

namespace Engine
{
    struct TransformComponent : std::enable_shared_from_this<TransformComponent>, Component
    {
    public:
        TransformMatrix Transform;

        std::shared_ptr<TransformComponent> Parent;
        std::vector<std::shared_ptr<TransformComponent>> Children;

        ~TransformComponent() = default;


        TransformComponent() : Component(typeid(*this).name()) {}

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

        TransformComponent(const TransformMatrix& transform, const std::shared_ptr<TransformComponent>& parent = nullptr) : Component(typeid(*this).name()), Transform(transform), Parent(parent)
        {
            NP_ENGINE_LOG_TRACE("We are not getting in here");
        };


        // lets you access the Transform, this is a glm::mat4 with extra fuss
         operator const TransformMatrix&() const { return Transform; }
         operator TransformMatrix&() { return Transform; }
    };
}
