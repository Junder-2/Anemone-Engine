#pragma once
#include "UILayerPanel.h"
#include "ANE/Math/Types/Vector3.h"
#include <ImGuizmo.h>

namespace Engine
{
    class EditorLayer;

    class ViewportPanel final : public UILayerPanel
    {
    public:
        ViewportPanel(EditorLayer* editorLayer);
        ~ViewportPanel();

        UIUpdateWrapper OnPanelRender() override;

    private:
        void ViewMenu();
        void ViewTransformMenu();
        static void ViewPhysicsDebugMenu();

        void DrawGizmos();

    private:
        bool _initialized = false;

        bool _useSnap = false;
        Vector3 _snapValues = .1f;
        Vector3 _snapRotationValues = 45.f;
        ImGuizmo::MODE _transformSpace = ImGuizmo::LOCAL;
        ImGuizmo::OPERATION _transformOperation = ImGuizmo::UNIVERSAL;

        EditorLayer* _editorLayer;
    };
}
