#include "nppch.h"
#include "Scene.h"

#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"

namespace Engine
{
    static void UpdatePosition(TransformMatrix& transform)
    {
        // I need to figure how the math library works better, don't know the methods yet
        //transform = scale(transform, glm::vec<3, float>(2, 1, 1));
    }

    static void OnTransformConstructed(entt::registry& registry, entt::entity entity)
    {
        NP_ENGINE_LOG_INFO("entity");
    }


    Engine::Scene::Scene()
    {
        //create an entity
        entt::entity entity = _registry.create();
       // entt::entity entit2 = _registry.create();

        // auto component2 = _registry.emplace<TransformComponent>(entit2);
        // add a component to an entity
        //auto component = _registry.emplace<TransformComponent>(entity, TransformMatrix(), component2);
        // component.SetParent(component2);
        //
        // // adds a component and calls the constructor
        auto componentChanged = _registry.emplace<TransformComponent>(entity, TransformMatrix());
        NP_ENGINE_LOG_TRACE(componentChanged.Name);
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


    void Scene::OnUpdate(float timeStep)
    {
    }
}
