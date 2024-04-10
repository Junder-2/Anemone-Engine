#include "anepch.h"
#include "Layer.h"

void Engine::Layer::AttachUIPanel(UILayerPanel* newPanel)
{
  _UIpanels.emplace(_UIpanels.begin(),newPanel);
}
void Engine::Layer::DetachUIPanel(UILayerPanel* panelToRemove)
{
    auto it = std::find(_UIpanels.begin(), _UIpanels.end(), panelToRemove);

    _UIpanels.erase(it);


}
