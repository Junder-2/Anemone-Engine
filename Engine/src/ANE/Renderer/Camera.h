#pragma once

namespace Engine
{
    struct TransformMatrix;
    struct CameraComponent;
    class Scene;

    class Camera
    {
    public:
        static bool HasCamera() { return _currentCameraComponent && _currentCameraTransform; }
        static CameraComponent& GetCameraComponent() { return *_currentCameraComponent; }
        static TransformMatrix& GetCameraTransform() { return *_currentCameraTransform; }

        static void UpdateCamera(Scene* scene);
        static void SubmitCamera();

        static void MarkPriorityDirty() { _priorityDirty = true; }
        static bool IsPriorityDirty() { return _priorityDirty; }

    private:
        inline static bool _priorityDirty = true;
        inline static CameraComponent* _currentCameraComponent = nullptr;
        inline static TransformMatrix* _currentCameraTransform = nullptr;
    };
}
