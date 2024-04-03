#pragma once
#include "../../Events/Event.h"

namespace Engine
{
    class NP_API Layer // should probaly be abstract
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnEvent(Event& event) = 0;

    protected:
        std::string _debugName;

    };
}
