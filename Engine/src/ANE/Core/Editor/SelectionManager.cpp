#include "anepch.h"
#include "SelectionManager.h"


void Engine::SelectionManager::RegisterSelect(const selectionContext ClickContextOrigin,const std::string clickedEntityUUID)
{
    _context = ClickContextOrigin;
    _selectedEntityUUID = clickedEntityUUID;
    S_Contexts[ClickContextOrigin].push_back(clickedEntityUUID);
}

void Engine::SelectionManager::DeSelect(const selectionContext ClickContextOrigin)
{
    _context = ClickContextOrigin;
    S_Contexts[ClickContextOrigin].clear();
}

std::vector<std::string>* Engine::SelectionManager::GetSelection(selectionContext _callingContext)
{
    std::vector<std::string>* contextIds = &S_Contexts[_callingContext];
    return contextIds;
}
