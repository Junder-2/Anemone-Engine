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
        auto it = std::ranges::find_if(_uiPanels, [newPanel](const UILayerPanel* panel)
        {
            return typeid(*panel) == typeid(*newPanel);
        });

        if (it == _uiPanels.end()) _uiPanels.emplace(_uiPanels.begin(), newPanel);
    }

    void Layer::DetachUIPanel(UILayerPanel* panelToRemove)
    {
        auto it = std::ranges::find(_uiPanels, panelToRemove);
        if (it != _uiPanels.end()) _uiPanels.erase(it);
    }
}
