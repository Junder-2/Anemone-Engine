#pragma once

namespace Engine
{
    class UILayerPanel;

    struct UIUpdateWrapper
    {
        UILayerPanel* RemoveSelf;
        std::vector<UILayerPanel*> PanelsToAdd;

        UIUpdateWrapper() : RemoveSelf(nullptr) {}

        UIUpdateWrapper(const UIUpdateWrapper& other) = default;

        void AddPanel(UILayerPanel* panelToAdd);

        void Clean()
        {
            RemoveSelf = nullptr;
            PanelsToAdd.clear();
        }

        ~UIUpdateWrapper() = default;
    };
}
