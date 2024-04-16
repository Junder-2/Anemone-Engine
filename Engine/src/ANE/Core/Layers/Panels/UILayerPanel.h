#pragma once

namespace Engine
{
    class UILayerPanel
    {
    protected:
        ~UILayerPanel() = default;
    public:

        virtual void OnPanelRender();


        bool _isVisible = true;
        bool _isEnabled = true;

    };
}
