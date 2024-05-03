#pragma once
#include <string> //these includes are necessary for compilation in sandbox
#include <vector> //these includes are necessary for compilation in sandbox

namespace Engine
{
    class UILayerPanel;
    class Event;

    class  Layer // should probaly be abstract
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
        void DetachUIPanel(UILayerPanel* panelToRemove);

    protected:
        std::vector<UILayerPanel*> _UIpanels; // todo: make into smartpointer
        std::string _debugName;
    };
}
