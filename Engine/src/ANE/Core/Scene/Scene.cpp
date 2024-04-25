#include "anepch.h"
#include "Scene.h"

#include "ANE/Core/Entity/Entity.h"
#include "ANE/Math/FMath.h"
#include "Components/NativeScriptComponent.h"
#include "Components/RenderComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/UUIDComponent.h"

namespace Engine
{
    Scene::Scene()
    {
        _physicsWorld = &GetPhysicsSystem().GetPhysicsWorld();

        //create an entity
        //entt::entity entity = Registry.create();
        // entt::entity entit2 = _registry.create();

        //Entity ent(this);

        // auto component = ent.AddComponent<RenderComponent>();
        // //auto Gety = ent.TryGetComponent<RenderComponent>(comp);
        // if (RenderComponent comp; ent.TryGetComponent(comp))
        // {
        //     ANE_ELOG("WE have component: {}", comp.ToString());
        // }
        // else
        // {
        //     ANE_ELOG("We do not have component");
        // }
        // auto component = ent.GetComponent<TransformComponent>();
        // if (ent.HasComponent<TransformComponent>())
        // {
        //
        // }

        // if (componentR nullptr)
        // {
        //     ANE_ELOG("We have component");
        // }
        // else
        // {
        //     ANE_ELOG("We don't have component");
        // }
        // auto component2 = _registry.emplace<TransformComponent>(entit2);
        // add a component to an entity
        //auto component = _registry.emplace<TransformComponent>(entity, TransformMatrix(), component2);
        // component.SetParent(component2);
        //
        // // adds a component and calls the constructor
        //auto componentChanged = Registry.emplace<TransformComponent>(entity, TransformMatrix());
        //componentChanged.ToString();
        //ANE_ELOG(componentChanged.ToString());

        //
        // // calls a method on the component
        // UpdatePosition(component);
        //
        // // removes the a component
        // //_registry.remove<TransformComponent>(entity);
        //
        // // removes all of the type of components
        // // _registry.clear<TransformComponent>();
        //
        // // gets the component from the entity
        // auto fetchedComponent = _registry.get<TransformComponent>(entity);

        // gets all entity with attached component
        // auto const view = _registry.view<TransformComponent>();
        // for (auto entt : view)
        // {
        //     // we iterate over every entity with component and use view to get component
        //     UpdatePosition(view.get<TransformComponent>(entt));
        // }
        //
        // // used to fetch multiple components and group them.
        // auto group = _registry.group<TransformComponent>(entt::get<RenderComponent>);
        //
        // // makes two handles that can be used
        // for (auto entt : group)
        // {
        //     // miss interformaiotn
        //     auto [transform, renderer] = group.get<TransformComponent, RenderComponent>(entt);
        // }
        //
        // // Subscribes to events
        // _registry.on_construct<TransformComponent>().connect<&OnTransformConstructed>();
    }


    void Scene::OnEvent(Event& e)
    {
    }

    void Scene::OnUpdate(float timeStep)
    {
        {
            _registry.view<NativeScriptComponent>().each([&](auto entity, auto& scriptComponent)
            {
                if (!scriptComponent.Instance)
                {
                    scriptComponent.Instantiate();
                    scriptComponent.Instance->_entity = { entity, this };
                    scriptComponent.OnCreateFunction();
                }
                scriptComponent.OnUpdateFunction(timeStep);
            });
        }

        _accumulator += timeStep;

        // Fixed update
        while (_accumulator >= _timeStep)
        {
            _accumulator = FMath::Max0(_accumulator - _timeStep);

            OnFixedUpdate(_timeStep);
        }

        UpdateRigidBodies();

        SubmitDrawCommands();
    }

    void Scene::OnFixedUpdate(float timeStep)
    {
        const auto group = _registry.view<TransformComponent, RigidBodyComponent>();
        for (const auto entity : group) //We need to apply changes in our transform to the internal rigidbody
        {
            auto[transform, body] = group.get<TransformComponent, RigidBodyComponent>(entity);

            TransformMatrix& transformMatrix = transform.Transform;

            if(!transformMatrix.IsDirty()) continue;

            body.GetRigidBody()->SetTransform(transformMatrix.GetPosition(), transformMatrix.GetQuaternion());
            transformMatrix.ClearDirty();
        }

        _physicsWorld->update(timeStep);

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

    void Scene::UpdateRigidBodies()
    {
        const float factor = FMath::Saturate(_accumulator / _timeStep);

        const auto group = _registry.view<TransformComponent, RigidBodyComponent>();
        for (const auto entity : group)
        {
            auto[transform, body] = group.get<TransformComponent, RigidBodyComponent>(entity);

            TransformMatrix& transformMatrix = transform.Transform;

            if(!body.GetRigidBody()->IsActive()) continue;

            auto currentTransform = rp3d::Transform(transformMatrix.GetPosition(), transformMatrix.GetQuaternion());
            auto newTransform = rp3d::Transform::interpolateTransforms(currentTransform, body.GetRigidBody()->GetReactRigidBody().getTransform(), factor);

            transformMatrix.SetPosition(Vector3::Convert(newTransform.getPosition()));
            transformMatrix.SetRotation(Quaternion::Convert(newTransform.getOrientation()));
            transformMatrix.ClearDirty();
        }
    }

    void Scene::SubmitDrawCommands()
    {
        // TODO: Sort based on pivot or bounds.
        const auto view = _registry.view<TransformComponent, RenderComponent>();
        for (entt::entity entity : view)
        {
            auto [transform, renderer] = view.get<TransformComponent, RenderComponent>(entity);

            DrawCommand draw = {};
            draw.ModelMatrix = transform.Transform.GetLocalToWorld();
            draw.VertexCount = renderer.Model.NumVertices;
            draw.MeshBuffers = renderer.Model.MeshBuffers;

            Renderer::SubmitDrawCommand(draw);
        }
        // Todo: add triangle or line rendering pipeline
        // if(_physicsWorld)
        // {
        //     rp3d::DebugRenderer physicsDebugRenderer = _physicsWorld->getDebugRenderer();
        //
        //     DrawCommand draw = {};
        //     draw.ModelMatrix = Matrix4x4::Identity();
        //     draw.VertexCount = physicsDebugRenderer.getNbLines();
        //     draw.MeshBuffers = renderer.Model.MeshBuffers;
        //
        //     Renderer::SubmitDrawCommand(draw);
        // }
    }

    /**
     * \brief Creates a Entity in the scene, adds a Transform and Tag
     * \param name Name of Entity, if no name is given it will be tagged with: "Untagged"
     * \return reference of the newly created Entity.
     */
    [[nodiscard("Entity never used")]] Entity Scene::Create(const char* name)
    {
        Entity ent{this, name};
        _entityMap[ent.GetComponent<UUIDComponent>().UUID] = ent;// here
        return ent;
    }
    [[nodiscard("Entity never used")]] Entity Scene::Create(std::string stringName)
    {

        Entity ent{this, stringName.c_str()};
        _entityMap[ent.GetComponent<UUIDComponent>().UUID] = ent;
        return ent;
    }

    Entity Scene::GetEntityWithUUID(std::string UUID)
    {
        return _entityMap[UUID];
    }

}
