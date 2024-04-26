#include "anepch.h"
#include "SelectionManager.h"

namespace Engine
{
    void SelectionManager::RegisterSelect(const SelectionContext clickContextOrigin, const std::string& clickedEntityUUID)
    {
        Context = clickContextOrigin;
        SelectedEntityUUID = clickedEntityUUID;
        SelectionContexts[clickContextOrigin].push_back(clickedEntityUUID);
    }

    void SelectionManager::DeSelect(const SelectionContext clickContextOrigin)
    {
        Context = clickContextOrigin;
        SelectionContexts[clickContextOrigin].clear();
    }

    std::vector<std::string>* SelectionManager::GetSelection(const SelectionContext callingContext)
    {
        std::vector<std::string>* contextIds = &SelectionContexts[callingContext];
        return contextIds;
    }
}
