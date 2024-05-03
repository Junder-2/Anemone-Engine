#include "anepch.h"
#include "Layer.h"

#include "Panels/UILayerPanel.h"

namespace Engine
{
    void Layer::OnUpdate(float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();
    }

    void Layer::OnEvent(Event& e)
    {
        ANE_DEEP_PROFILE_FUNCTION();
    }

    void Layer::OnUIRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();
    }

    void Layer::AttachUIPanel(UILayerPanel* newPanel)
    {
        auto it = std::ranges::find_if(_UIpanels, [newPanel](const UILayerPanel* panel)
        {
            return typeid(*panel) == typeid(*newPanel);
        });

        if (it == _UIpanels.end()) _UIpanels.emplace(_UIpanels.begin(), newPanel);
    }

    void Layer::DetachUIPanel(UILayerPanel* panelToRemove)
    {
        auto it = std::ranges::find(_UIpanels, panelToRemove);
        if (it != _UIpanels.end()) _UIpanels.erase(it);
    }
}
