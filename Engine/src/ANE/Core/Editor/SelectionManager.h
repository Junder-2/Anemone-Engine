#pragma once
#include <entt.hpp>

namespace Engine
{
    class SelectionManager
    {
    public:

        enum selectionContext
        {
            Global = 0,
            SceneViewport = 1,
            UI = 2,
        };
        inline static selectionContext _context = Global;
        inline static std::string _selectedEntityUUID;
        static void RegisterSelect(const selectionContext ClickContextOrigin,std::string UUID);
        static void DeSelect(const selectionContext ClickContextOrigin);
        inline static std::unordered_map<selectionContext,std::vector<std::string>> S_Contexts;
        static std::vector<std::string>* GetSelection(selectionContext _callingContext);

    };
}
