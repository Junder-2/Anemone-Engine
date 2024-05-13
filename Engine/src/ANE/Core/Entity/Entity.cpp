#include "anepch.h"
#include "Entity.h"

#include "UUIDGenerator.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"
#include "ANE/Core/Scene/Components/Components.h"

Engine::Entity::Entity(Scene* scene, const char* name)
{
    _sceneHandle = scene;

    _entityHandle = _sceneHandle->_registry.create();
    //this->AddComponent<AttachmentsComponent>();
    this->AddComponent<UUIDComponent>(UUIDGenerator::GetUUID());
    this->AddComponent<TagComponent>(name);
    this->AddComponent<TransformComponent>();
}


