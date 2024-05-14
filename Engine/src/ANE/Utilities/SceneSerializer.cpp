#include "anepch.h"
#include "SceneSerializer.h"

#include "ANE/Core/Entity/ExampleScripts/CameraController.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Scene/Components/Components.h"
#include "ANE/Physics/Physics.h"


namespace Engine
{
    SceneSerializer::SceneSerializer()
    {
    }

    toml::table SceneSerializer::EntitySerializer(Entity& entity)
    {
        toml::table componentTable;

        if (TagComponent tagComp; entity.TryGetComponent(tagComp))
        {
            auto table = toml::table{
                {"_value", tagComp.Value}
            };
            componentTable.insert_or_assign("_tag", table);
        }

        if (TransformComponent transformComp; entity.TryGetComponent(transformComp))
        {
            auto table = toml::table{
                {
                    "_position", toml::array{
                        transformComp.Transform.GetPosition().X,
                        transformComp.Transform.GetPosition().Y,
                        transformComp.Transform.GetPosition().Z
                    }
                },
                {
                    "_rotation", toml::array{
                        transformComp.Transform.GetEulerAngles().X,
                        transformComp.Transform.GetEulerAngles().Y,
                        transformComp.Transform.GetEulerAngles().Z
                    }
                },
                {
                    "_scale", toml::array{
                        transformComp.Transform.GetScale().X,
                        transformComp.Transform.GetScale().Y,
                        transformComp.Transform.GetScale().Z
                    }
                }
            };

            componentTable.insert_or_assign("_transformComponent", table);
        }

        if (CameraComponent cameraComp; entity.TryGetComponent(cameraComp))
        {
            auto table = toml::table{
                {"_aspectRatio", cameraComp.GetAspectRatio()},
                {"_fieldOfView", cameraComp.GetFOV()},
                {"zNear", cameraComp.GetNearClip()},
                {"zFar", cameraComp.GetFarClip()}
            };
            componentTable.insert_or_assign("_cameraComponent", table);
        }

        if (ColliderComponent colliderComp; entity.TryGetComponent(colliderComp))
        {
            toml::array colliders;
            for (auto collider : colliderComp.GetColliders())
            {
                toml::table payload;
                switch (collider->GetShapeType())
                {
                    case CollisionShapeType::Box:
                    {
                        auto castedBoxCollider = reinterpret_cast<BoxCollider*>(collider);
                        payload.insert_or_assign(
                            "_halfSize", toml::array{
                                castedBoxCollider->GetHalfSize().X,
                                castedBoxCollider->GetHalfSize().Y,
                                castedBoxCollider->GetHalfSize().Z
                            }
                        );
                        break;
                    }
                    case CollisionShapeType::Sphere:
                    {
                        auto castedSphereCollider = reinterpret_cast<SphereCollider*>(collider);
                        payload.insert_or_assign("_radius", castedSphereCollider->GetRadius());
                        break;
                    }
                    case CollisionShapeType::Capsule:
                    {
                        auto castedCapsuleCollider = reinterpret_cast<CapsuleCollider*>(collider);

                        payload.insert_or_assign("_radius", castedCapsuleCollider->GetRadius());
                        payload.insert_or_assign("_height", castedCapsuleCollider->GetHeight());
                        break;
                    }
                }

                auto table = toml::table{
                    {
                        "_colliderPosition", toml::array{
                            collider->GetPosition().X,
                            collider->GetPosition().Y,
                            collider->GetPosition().Z
                        }
                    },
                    {
                        "_colliderRotation", toml::array{
                            collider->GetEulerAngles().X,
                            collider->GetEulerAngles().Y,
                            collider->GetEulerAngles().Z
                        }
                    },
                    {
                        "_colliderMaterial",
                        toml::array{
                            collider->GetMaterial().getFrictionCoefficient(),
                            collider->GetMaterial().getBounciness(),
                            collider->GetMaterial().getMassDensity()
                        }
                    },
                    {"_colliderCollisionMask", collider->GetCollisionMask()},
                    {"_colliderCollisionCategories", collider->GetCollisionCategories()},
                    {"_shapeType", collider->GetShapeType()},
                    {"_colliderSpecificData", payload}
                };

                colliders.emplace_back(table);
            }

            auto table = toml::table{
                {"_colliders", colliders}
            };
            componentTable.insert_or_assign("_colliderComponent", table);
        }

        if (RenderComponent renderComp; entity.TryGetComponent(renderComp))
        {
            auto table = toml::table{
                {"_modelPath", renderComp.GetModelPath()}
            };

            componentTable.insert_or_assign("_renderComponent", table);
        }

        if (RigidBodyComponent rigidComp; entity.TryGetComponent(rigidComp))
        {
            auto table = toml::table{
                {"_bodyType", rigidComp.GetRigidBody()->GetBodyType()},
                {"_useGravity", rigidComp.GetRigidBody()->IsGravityEnabled()},
                {"_mass", rigidComp.GetRigidBody()->GetMass()},
                {"_active", rigidComp.GetRigidBody()->IsActive()}
            };
            componentTable.insert_or_assign("_rigidBodyComponent", table);
        }

        // difficult
        // NativeScriptComponent scriptComp;
        // if (entity.TryGetComponent(scriptComp))
        // {
        //     toml::table table{
        //         {"_aspectRatio", cameraComp.GetAspectRatio()},
        //         {"_fieldOfView", cameraComp.GetFOV()},
        //         {"zNear", cameraComp.GetNearClip()},
        //         {"zFar", cameraComp.GetFarClip()}
        //     };
        //     componentArray.emplace_back(table);
        // }

        return componentTable;
    }

    void SceneSerializer::Serialize(const std::shared_ptr<Scene>& scene)
    {
        toml::array arr;

        for (auto entity : scene->_registry.view<TagComponent>())
        {
            Entity ent = {entity, scene.get()};

            arr.emplace_back(EntitySerializer(ent));
        }

        auto table = toml::table{
            {"Scene", scene->Name},
            {"Entities", arr}
        };

        std::string path = _filepath + scene->Name + ".toml";

        WriteToFile(path, table);
    }

    void SceneSerializer::SerializeBinary()
    {
    }

    bool SceneSerializer::HasFile(const char* fileName)
    {
        std::string filePath = _filepath + fileName + ".toml";
        return std::filesystem::is_regular_file(filePath);
    }

    std::shared_ptr<Scene> SceneSerializer::CreateEmptySceneFile(const char* sceneName)
    {
        std::string filePath = _filepath + sceneName + ".toml";

        std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneName);
        auto table = toml::table{
            {"Scene", scene->Name},
            {"Entities", toml::array{}}
        };

        WriteToFile(filePath, table);

        return scene;
    }

    std::shared_ptr<Scene> SceneSerializer::Deserialize(const char* key, EditorLayer* layer) const
    {
        std::string filePath = _filepath + key + ".toml";

        toml::table table;

        try
        {
            table = toml::parse_file(filePath);
        }
        catch (const toml::parse_error& err)
        {
            ANE_ELOG(err.description());
        }

        const auto sceneName = table["Scene"].value_or("NoSceneFound");

        auto scene = std::make_shared<Scene>(sceneName);

        if (table.empty()) return scene;

        auto nodeView = table["Entities"];

        if (toml::array* entityArray = nodeView.as_array())
        {
            for (auto& entity : *entityArray)
            {
                if (const auto& componentTable = entity.as_table())
                {
                    auto tagComponent = (*componentTable)["_tag"]["_value"].value<std::string>();

                    auto entityRef = scene->Create(*tagComponent);

                    if (auto transformComponent = (*componentTable)["_transformComponent"]; !!transformComponent)
                    {
                        auto& entityTransformComp = entityRef.AddComponent<TransformComponent>();
                        auto position = transformComponent["_position"];
                        entityTransformComp.Transform.SetPosition(Vector3(*position[0].value<float>(), *position[1].value<float>(), *position[2].value<float>()));

                        auto rotation = transformComponent["_rotation"];
                        entityTransformComp.Transform.SetRotation(Vector3(*rotation[0].value<float>(), *rotation[1].value<float>(), *rotation[2].value<float>()));

                        auto scale = transformComponent["_scale"];
                        entityTransformComp.Transform.SetScale(Vector3(*scale[0].value<float>(), *scale[1].value<float>(), *scale[2].value<float>()));
                    }

                    if (auto cameraComponent = (*componentTable)["_cameraComponent"]; !!cameraComponent)
                    {
                        auto& entityCameraComponent = entityRef.AddComponent<CameraComponent>();
                        entityCameraComponent.SetPerspective(*cameraComponent["_fieldOfView"].value<float>(),
                                                             *cameraComponent["_aspectRatio"].value<float>(),
                                                             *cameraComponent["zNear"].value<float>(),
                                                             *cameraComponent["zFar"].value<float>());

                        entityRef.AddComponent<NativeScriptComponent>().Bind<CameraController>();
                    }

                    if (auto rigidBodyComponent = (*componentTable)["_rigidBodyComponent"]; !!rigidBodyComponent)
                    {
                        auto& entityRigidBodyComponent = entityRef.AddComponent<RigidBodyComponent>(entityRef);
                        entityRigidBodyComponent.GetRigidBody()->SetMass(*(*componentTable)["_rigidBodyComponent"]["_mass"].value<float>());
                        entityRigidBodyComponent.GetRigidBody()->SetBodyType(static_cast<BodyType>(*(*componentTable)["_rigidBodyComponent"]["_bodyType"].value<int>()));
                        entityRigidBodyComponent.GetRigidBody()->SetActive(*(*componentTable)["_rigidBodyComponent"]["_active"].value<bool>());
                        entityRigidBodyComponent.GetRigidBody()->SetUseGravity(*(*componentTable)["_rigidBodyComponent"]["_useGravity"].value<bool>());
                    }

                    if (auto colliderComponent = (*componentTable)["_colliderComponent"]; !!colliderComponent)
                    {
                        auto& entityColliderComp = entityRef.AddComponent<ColliderComponent>(entityRef);
                        auto collidersView = colliderComponent["_colliders"];
                        if (toml::array* colliders = collidersView.as_array())
                        {
                            for (auto& collider : *colliders)
                            {
                                if (toml::table* colliderTable = collider.as_table())
                                {
                                    Collider* col;

                                    switch (*(*colliderTable)["_shapeType"].value<int>())
                                    {
                                        case 0:
                                        {
                                            auto radius = (*colliderTable)["_colliderSpecificData"]["_radius"].value<float>();
                                            col = entityColliderComp.AddSphereCollider(*radius);
                                        }
                                        break;
                                        case 1:
                                        {
                                            Vector3 halfExtends = Vector3(*(*colliderTable)["_colliderSpecificData"]["_halfSize"][0].value<float>(), *(*colliderTable)["_colliderSpecificData"]["_halfSize"][1].value<float>(), *(*colliderTable)["_colliderSpecificData"]["_halfSize"][2].value<float>());
                                            col = entityColliderComp.AddBoxCollider(halfExtends);
                                        }
                                        break;
                                        case 2:
                                        {
                                            auto radius = (*colliderTable)["_colliderSpecificData"]["_radius"].value<float>();
                                            auto height = (*colliderTable)["_colliderSpecificData"]["_height"].value<float>();
                                            col = entityColliderComp.AddCapsuleCollider(*radius, *height);
                                        }
                                        break;
                                        default: ;
                                    }
                                    if (col != nullptr)
                                    {
                                        col->SetCollisionMask(static_cast<CollisionLayerMask>(*(*colliderTable)["_colliderCollisionMask"].value<int>()));
                                        col->SetCollisionCategories(static_cast<CollisionLayerMask>(*(*colliderTable)["_colliderCollisionCategories"].value<int>()));
                                        col->GetMaterial().setFrictionCoefficient(*(*colliderTable)["_colliderMaterial"][0].value<float>());
                                        col->GetMaterial().setBounciness(*(*colliderTable)["_colliderMaterial"][1].value<float>());
                                        col->GetMaterial().setMassDensity(*(*colliderTable)["_colliderMaterial"][2].value<float>());
                                        col->SetPosition(Vector3(*(*colliderTable)["_colliderPosition"][0].value<float>(), *(*colliderTable)["_colliderPosition"][1].value<float>(), *(*colliderTable)["_colliderPosition"][2].value<float>()));
                                        col->SetRotation(Vector3(*(*colliderTable)["_colliderRotation"][0].value<float>(), *(*colliderTable)["_colliderRotation"][1].value<float>(), *(*colliderTable)["_colliderRotation"][2].value<float>()));
                                    }
                                }
                            }
                        }
                    }

                    if (auto renderComponent = (*componentTable)["_renderComponent"]; !!renderComponent)
                    {
                        entityRef.AddComponent<RenderComponent>(*(*componentTable)["_renderComponent"]["_modelPath"].value<std::string>());
                    }

                    // ScriptComponent
                }
            }
        }


        return scene;
    }

    std::shared_ptr<Scene> SceneSerializer::DeserializeBinary()
    {
        // not implemented
        ANE_ASSERT(false);

        Scene scene{"key"};

        return std::make_shared<Scene>("key");
    }

    void SceneSerializer::WriteToFile(std::string filepath, const toml::table& content)
    {
        std::ofstream output(filepath, std::ios::trunc);

        if (!output.is_open()) output.open(filepath);

        output << content << "\n\n";

        output.close();
    }

    void SceneSerializer::WriteToFile(std::string filepath)
    {
        std::ofstream output(filepath, std::ios::app);

        if (!output.is_open()) output.open(filepath);

        output.close();
    }
}
