#pragma once
#include "ANE/Events/Event.h"

namespace Engine
{
    class ANE_API Layer // should probaly be abstract
    {
    public:
        Layer(const std::string& name = "Layer") : _debugName(name) { }
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnEvent(Event& e) {}
        virtual void OnUIRender() {}

    protected:
        std::string _debugName;

    };
}
