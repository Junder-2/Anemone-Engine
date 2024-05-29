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

        void SetCurrentTransformSpace(ImGuizmo::MODE mode);
        void SetCurrentTransformOperation(ImGuizmo::OPERATION operation);

    private:
        bool _initialized = false;

        bool _useSnap = false;
        Vector3 _snapValues = .1f;
        Vector3 _snapRotationValues = 45.f;

        int _currentTransformSpaceOption = 0;
        ImGuizmo::MODE _transformSpace = ImGuizmo::WORLD;
        int _currentTransformOperation = 0;
        ImGuizmo::OPERATION _transformOperation = ImGuizmo::UNIVERSAL;

        EditorLayer* _editorLayer;
    };
}
