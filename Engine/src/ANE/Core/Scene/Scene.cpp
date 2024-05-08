#include "anepch.h"
#include "Scene.h"

#include "ANE/Math/FMath.h"
#include "ANE/Physics/Physics.h"
#include "Components/Components.h"

namespace Engine
{
    Scene::Scene()
    {
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
        ANE_DEEP_PROFILE_FUNCTION();
    }

    void Scene::OnUpdate(float timeStep)
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

        GetPhysicsSystem().UpdateRigidBodies(FMath::Saturate(_accumulator / _timeStep), this);

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
    }
}
