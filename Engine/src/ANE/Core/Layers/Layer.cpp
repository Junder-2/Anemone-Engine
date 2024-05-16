#include "anepch.h"
#include "Layer.h"

#include "Panels/UILayerPanel.h"

namespace Engine
{
    void Layer::AttachUIPanel(UILayerPanel* newPanel)
    {
        ANE_PROFILE_FUNCTION();

        auto it = std::ranges::find_if(_uiPanels, [newPanel](const UILayerPanel* panel)
        {
            return typeid(*panel) == typeid(*newPanel);
        });

        if (it == _uiPanels.end()) _uiPanels.emplace(_uiPanels.begin(), newPanel);
    }

    void Layer::DetachUIPanel(UILayerPanel* panelToRemove)
    {
        ANE_PROFILE_FUNCTION();

        auto it = std::ranges::find(_uiPanels, panelToRemove);
        if (it != _uiPanels.end()) _uiPanels.erase(it);
    }
}
