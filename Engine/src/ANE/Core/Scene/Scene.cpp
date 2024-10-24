﻿#include "anepch.h"
#include "Scene.h"

#include "ANE/Math/FMath.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Renderer/Camera.h"
#include "ANE/Utilities/API.h"
#include "Components/Components.h"
#include "Components/LightComponent.h"

namespace Engine
{
    Scene::Scene(std::string sceneName) : Name(sceneName)
    {

    }

    /**
    * \brief Creates a Entity in the scene, adds a Transform and Tag
    * \param name Name of Entity, if no name is given it will be tagged with: "Untagged"
    * \return reference of the newly created Entity.
    */
    [[nodiscard("Entity never used")]] Entity Scene::Create(const char* name)
    {
        Entity ent{this, name};
        _entityMap[ent.GetComponent<UUIDComponent>().UUID] = ent; // here
        return ent;
    }

    [[nodiscard("Entity never used")]] Entity Scene::Create(const std::string& stringName)
    {
        Entity ent{this, stringName.c_str()};
        _entityMap[ent.GetComponent<UUIDComponent>().UUID] = ent;
        return ent;
    }

    Entity Scene::GetEntityWithUUID(std::string UUID)
    {
        return _entityMap[UUID];
    }

    void Scene::OnEvent(Event& e)
    {
        ANE_DEEP_PROFILE_FUNCTION();
    }

    void Scene::OnUpdate(float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        {
            _registry.view<NativeScriptComponent>().each([&](auto entity, auto& scriptComponent)
            {
                if (!scriptComponent.Instance)
                {
                    scriptComponent.Instantiate();
                    scriptComponent.Instance->_entity = { entity, this };
                    scriptComponent.OnCreateFunction();
                }
                scriptComponent.OnUpdateFunction(deltaTime);
            });
        }

        _accumulator += deltaTime;

        const float physTimeStep = API::PHYSICS_TIMESTEP;

        // Fixed update
        while (_accumulator >= physTimeStep)
        {
            _accumulator = FMath::Max0(_accumulator - physTimeStep);

            OnFixedUpdate(physTimeStep);
        }

        GetPhysicsSystem().UpdateRigidBodies(FMath::Saturate(_accumulator / physTimeStep), this);

        Camera::UpdateCamera(this);

        SubmitDrawCommands();
    }

    void Scene::OnFixedUpdate(float timeStep)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        GetPhysicsSystem().PhysicsUpdate(timeStep, this);

        _registry.view<NativeScriptComponent>().each([&](auto entity, auto& scriptComponent)
        {
            if (!scriptComponent.Instance)
            {
                scriptComponent.Instantiate();
                scriptComponent.Instance->_entity = { entity, this };
                scriptComponent.OnCreateFunction();
            }
            scriptComponent.OnFixedUpdateFunction(timeStep);
        });
    }

    void Scene::SubmitDrawCommands()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        // TODO: Gather more lights for rendering.
        {
            Vector3 dirLightColor = 0.f;
            Vector3 dirLightDir = -Vector3::UpVector();

            const auto view = _registry.view<TransformComponent, LightComponent>();
            for (const auto entity : view)
            {
                auto [transform, light] = view.get<TransformComponent, LightComponent>(entity);

                if(light.GetLightType() != LightType::Directional) continue;

                dirLightColor = light.GetColor() * light.GetIntensity();
                dirLightDir = transform.Transform.GetForward();

                break; // We only care about the first dir light.
            }

            Renderer::SetSunColor(dirLightColor);
            Renderer::SetSunDirection(dirLightDir);
        }

        {
            // // Sort based on Transform square distance.
            // const Vector3 camPos = Camera::GetCameraTransform().GetPosition();
            // _registry.sort<TransformComponent>([&](const TransformComponent& lhs, const TransformComponent& rhs)
            // {
            //     const float sqrDstA = (camPos - lhs.Transform.GetPosition()).LengthSquare();
            //     const float sqrDstB = (camPos - rhs.Transform.GetPosition()).LengthSquare();
            //     return sqrDstA < sqrDstB;
            // });

            auto view = _registry.view<TransformComponent, RenderComponent>();
            view.use<TransformComponent>(); // Use iteration order of TransformComponent.
            for (const auto entity : view)
            {
                auto [transform, renderer] = view.get<TransformComponent, RenderComponent>(entity);

                DrawCommand draw = {};
                draw.ModelMatrix = transform.Transform.GetLocalToWorld();
                draw.VertexCount = renderer.Model->NumVertices;
                draw.MeshBuffers = renderer.Model->MeshBuffers;

                draw.Material = renderer.GetMaterial();

                Renderer::SubmitDrawCommand(draw);
            }
        }
    }
}
