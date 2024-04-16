#pragma once
#include "ANE/Events/Event.h"
#include "Panels/UILayerPanel.h"

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

        void AttachUIPanel(UILayerPanel* newPanel);
        void DetachUIPanel(UILayerPanel* panelToRemove);

        std::vector<UILayerPanel*> _UIpanels; // todo: make into smartpointer

    protected:
        std::string _debugName;

    };
}
