#pragma once
#include "UILayerPanel.h"
#include "ANE/Math/Types/Vector2.h"
#include "UIUpdateWrapper.h"

namespace Engine
{
    class EditorLayer;

    class CreateScenePanel : public UILayerPanel
    {
    public:
        CreateScenePanel(EditorLayer* layer);
        UIUpdateWrapper OnPanelRender() override;


    private:
        void CreateScene(const char* name) const;
        ~CreateScenePanel() = default;
        EditorLayer* _editorLayer;
        Vector2 _windowSize;
        Vector2 _windowLocation;

        const float WindowXSize = 300;
        const float WindowYSize = 120;

         std::string sceneName;
        char buffer[256] = {};
    };
}
