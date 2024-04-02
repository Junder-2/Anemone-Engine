#pragma once

namespace Engine
{
    class NP_API Layer // should probaly be abstract
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() { }
        virtual void OnDetach() { }
        virtual void OnUpdate(float deltaTime) { }


    protected:
        std::string _debugName;

    };
}
