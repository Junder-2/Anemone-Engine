#include "anepch.h"
#include "Camera.h"

#include "ANE/Renderer/Renderer.h"
#include "ANE/Core/Scene/Scene.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"
#include "ANE/Math/Types/TransformMatrix.h"

namespace Engine
{
    void Camera::UpdateCamera(Scene* scene)
    {
        if(IsPriorityDirty() || !HasCamera())
        {
            int currPriority = 10000;
            _currentCameraComponent = nullptr;
            _currentCameraTransform = nullptr;
            for (const auto view = scene->_registry.view<CameraComponent, TransformComponent>(); const auto entity : view)
            {
                auto [cam, transform] = view.get<CameraComponent, TransformComponent>(entity);
                if(cam.GetPriority() < currPriority)
                {
                    _currentCameraComponent = &cam;
                    _currentCameraTransform = &transform.Transform;
                    currPriority = _currentCameraComponent->GetPriority();
                }
            }

            _priorityDirty = false;
        }

        SubmitCamera();
    }

    void Camera::SubmitCamera()
    {
        if(!HasCamera()) return;

        Renderer::SetCameraPosition(_currentCameraTransform->GetPosition());
        if(_currentCameraComponent->IsDirty() || _currentCameraTransform->IsDirty())
        {
            const Matrix4x4 viewMatrix = _currentCameraTransform->GetWorldToLocal();
            const Matrix4x4 projMatrix = _currentCameraComponent->GetPerspectiveMatrix();
            Renderer::SetViewProjection(projMatrix * viewMatrix);
            _currentCameraComponent->ClearDirty();
            _currentCameraTransform->ClearDirty();
        }
    }
}
