#include "anepch.h"
#include "Layer.h"

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
        _UIpanels.emplace(_UIpanels.begin(),newPanel);
    }
    void Layer::DetachUIPanel(const UILayerPanel* panelToRemove)
    {
        const auto it = std::find(_UIpanels.begin(), _UIpanels.end(), panelToRemove);

        _UIpanels.erase(it);
    }
}
