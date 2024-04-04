#pragma once
#include "../../Events/Event.h"

namespace Engine
{
    class NP_API Layer // should probaly be abstract
    {
    public:
        Layer(const std::string& name = "Layer") : _debugName(name) { }
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnEvent(Event& e) {}

    protected:
        std::string _debugName;

    };
}
