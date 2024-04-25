#include "anepch.h"
#include "Layer.h"

namespace Engine
{
    void Layer::AttachUIPanel(UILayerPanel* newPanel)
    {
        _UIpanels.emplace(_UIpanels.begin(),newPanel);
    }
    void Layer::DetachUIPanel(const UILayerPanel* panelToRemove)
    {
        const auto it = std::find(_UIpanels.begin(), _UIpanels.end(), panelToRemove);

        _UIpanels.erase(it);
    }
}
