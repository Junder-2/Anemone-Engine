#pragma once
#include <entt.hpp>

namespace Engine
{
    class SelectionManager
    {
    public:
        enum SelectionContext
        {
            Global = 0,
            SceneViewport = 1,
            UI = 2,
        };

        static void RegisterSelect(SelectionContext clickContextOrigin, const std::string& uuid);
        static void DeSelect(SelectionContext clickContextOrigin);

        inline static SelectionContext Context = Global;
        inline static std::string SelectedEntityUUID;
        inline static std::unordered_map<SelectionContext, std::vector<std::string>> SelectionContexts;
        static std::vector<std::string>* GetSelection(SelectionContext callingContext);
    };
}
