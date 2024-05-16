#include "anepch.h"
#include "UIUpdateWrapper.h"
#include "UILayerPanel.h"

namespace Engine
{
    void UIUpdateWrapper::AddPanel(UILayerPanel* panelToAdd)
    {
        ANE_PROFILE_FUNCTION();

        auto it = std::ranges::find_if(PanelsToAdd, [panelToAdd](const UILayerPanel* panel)
        {
            return typeid(*panel) == typeid(*panelToAdd);
        });

        if (it == PanelsToAdd.end()) PanelsToAdd.emplace(PanelsToAdd.begin(), panelToAdd);
    }
}
