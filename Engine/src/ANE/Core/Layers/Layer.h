#pragma once

namespace Engine
{
    class UILayerPanel;
    class Event;

    class ANE_API Layer // should probaly be abstract
    {
    public:
        Layer(const std::string& name = "Layer") : _debugName(name) { }
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime);
        virtual void OnEvent(Event& e);
        virtual void OnUIRender();

        void AttachUIPanel(UILayerPanel* newPanel);
        void DetachUIPanel(const UILayerPanel* panelToRemove);

    protected:
        std::vector<UILayerPanel*> _UIpanels; // todo: make into smartpointer
        std::string _debugName;
    };
}
