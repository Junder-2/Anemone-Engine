#pragma once
#include <assimp/Logger.hpp>

#include "UILayerPanel.h"
#include "ANE/Core/Log/LogSinkBase.h"

namespace Engine
{
    class EditorLogPanel : public UILayerPanel
    {
    public:
        EditorLogPanel();
        ~EditorLogPanel();
        void OnPanelRender() override;

    };



}
