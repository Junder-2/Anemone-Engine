﻿#include "nppch.h"
#include "Scene.h"

#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"


static void UpdatePosition(const glm::mat4& transform) {
    // I need to figure how the math library works better, don't know the methods yet
    scale(transform, glm::vec<3, float>(2,1,1));
}

static void OnTransformConstructed(entt::registry& registry, entt::entity entity) {
    NP_ENGINE_LOG_INFO("entity");
}

Engine::Scene::Scene() {

    // create an entity
    entt::entity entity = _registry.create();

    // add a component to an entity
    auto component = _registry.emplace<TransformComponent>(entity);

    // adds a component and calls the constructor
    auto componentChanged = _registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

    // calls a method on the component
    UpdatePosition(component);

    // removes the a component
    _registry.remove<TransformComponent>(entity);

    // removes all of the type of components
    _registry.clear<TransformComponent>();

    // gets the component from the entity
    auto fetchedComponent = _registry.get<TransformComponent>(entity);

    // gets all entity with attached component
    auto const view = _registry.view<TransformComponent>();
    for (auto entt : view) {

        // we iterate over every entity with component and use view to get component
        UpdatePosition(view.get<TransformComponent>(entt));
    }

    // used to fetch multiple components and group them.
    auto group = _registry.group<TransformComponent>(entt::get<RenderComponent>);

    // makes two handles that can be used
    for (auto entt : group) {
        // cant get this one to work
        //auto&[transform, renderer] = group.get<TransformComponent, RenderComponent>(entt);
    }

    // Subscribes to events
    _registry.on_construct<TransformComponent>().connect<&OnTransformConstructed>();
}

Engine::Scene::~Scene() {

}