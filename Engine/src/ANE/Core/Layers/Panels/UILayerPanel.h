#pragma once
#include "UIUpdateWrapper.h"

namespace Engine
{
    class UILayerPanel
    {
    protected:
        ~UILayerPanel() = default;
    public:

        virtual UIUpdateWrapper OnPanelRender();

        void SetVisible(const bool enable) { _isVisible = enable; }
        bool IsVisible() const { return _isVisible; }

        void SetEnabled(const bool enable) { _isEnabled = enable; }
        bool IsEnabled() const { return _isEnabled; }

    protected:
        bool _isVisible = true;
        bool _isEnabled = true;
    };
}
