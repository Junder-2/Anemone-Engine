#include "anepch.h"
#include "SceneSerializer.h"

#include "ANE/Core/Entity/ExampleScripts/CameraController.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Scene/Components/Components.h"
#include "ANE/Physics/Physics.h"

namespace Engine
{
    #define WARN_NO_VALUE(message, ...) ANE_ELOG_WARN(message ", possible file corruption", (__VA_ARGS__))

    SceneSerializer::SceneSerializer()
    {
    }

    toml::table SceneSerializer::EntitySerializer(Entity& entity)
    {
        ANE_PROFILE_FUNCTION();

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
                        transformComp.Transform.GetEulerAngles(true).X,
                        transformComp.Transform.GetEulerAngles(true).Y,
                        transformComp.Transform.GetEulerAngles(true).Z
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
                            collider->GetEulerAngles(true).X,
                            collider->GetEulerAngles(true).Y,
                            collider->GetEulerAngles(true).Z
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

        if (LightComponent lightComp; entity.TryGetComponent(lightComp))
        {
            auto table = toml::table{
                {"_lightType", lightComp.GetLightType()},
                {"_intensity", lightComp.GetIntensity()},
                {"_color", toml::array{
                    lightComp.GetColor().R,
                    lightComp.GetColor().G,
                    lightComp.GetColor().B}}
            };

            componentTable.insert_or_assign("_lightComponent", table);
        }

        if (RigidBodyComponent rigidComp; entity.TryGetComponent(rigidComp))
        {
            auto table = toml::table{
                {"_bodyType", rigidComp.GetRigidBody()->GetBodyType()},
                {"_useGravity", rigidComp.GetRigidBody()->IsGravityEnabled()},
                {"_mass", rigidComp.GetRigidBody()->GetMass()},
                {"_damping", rigidComp.GetRigidBody()->GetDamping()},
                {"_angularDamping", rigidComp.GetRigidBody()->GetAngularDamping()},
                {"_useAutoMass", rigidComp.GetRigidBody()->IsAutoMass()},
                {"_useAutoCenterOfMass", rigidComp.GetRigidBody()->IsAutoCenterOfMass()},
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
        ANE_PROFILE_FUNCTION();

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

        std::string path = _filepath + scene->Name + ".anem";

        WriteToFile(path, table);
    }

    void SceneSerializer::SerializeBinary()
    {
    }

    bool SceneSerializer::HasFile(const char* fileName)
    {
        ANE_PROFILE_FUNCTION();

        std::string filePath = _filepath + fileName + ".anem";
        return std::filesystem::is_regular_file(filePath);
    }

    std::shared_ptr<Scene> SceneSerializer::CreateEmptySceneFile(const char* sceneName)
    {
        ANE_PROFILE_FUNCTION();

        std::string filePath = _filepath + sceneName + ".anem";

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
        ANE_PROFILE_FUNCTION();
        std::string filePath = _filepath + key + ".anem";

        toml::table table;

        try
        {
            table = toml::parse_file(filePath);
        }
        catch (const toml::parse_error& err)
        {
            ANE_ELOG(err.description());
        }

        ANE_ELOG_INFO("Begin deserializing file: {}", key);

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
                    if(!tagComponent)
                    {
                        WARN_NO_VALUE("Missing tagcomponent for entity skipping", 0);
                        continue;
                    }

                    auto entityRef = scene->Create(*tagComponent);

                    if (auto transformComp = (*componentTable)["_transformComponent"])
                    {
                        auto& entityTransform = entityRef.GetComponent<TransformComponent>().Transform;
                        if(auto position = transformComp["_position"])
                            entityTransform.SetPosition(Vector3(*position[0].value<float>(), *position[1].value<float>(), *position[2].value<float>()));
                        else WARN_NO_VALUE("TransformComp: _position doesnt exist for entity: {}", *tagComponent);

                        if(auto rotation = transformComp["_rotation"])
                            entityTransform.SetRotation(Vector3(*rotation[0].value<float>(), *rotation[1].value<float>(), *rotation[2].value<float>()), true);
                        else WARN_NO_VALUE("TransformComp: _rotation doesnt exist for entity: {}", *tagComponent);

                        if(auto scale = transformComp["_scale"])
                            entityTransform.SetScale(Vector3(*scale[0].value<float>(), *scale[1].value<float>(), *scale[2].value<float>()));
                        else WARN_NO_VALUE("TransformComp: _scale doesnt exist for entity: {}", *tagComponent);
                    }

                    if (auto camComp = (*componentTable)["_cameraComponent"])
                    {
                        auto& entityCameraComponent = entityRef.AddComponent<CameraComponent>();
                        entityCameraComponent.SetPerspective(*camComp["_fieldOfView"].value<float>(),
                                                             *camComp["_aspectRatio"].value<float>(),
                                                             *camComp["zNear"].value<float>(),
                                                             *camComp["zFar"].value<float>());

                        entityRef.AddComponent<NativeScriptComponent>().Bind<CameraController>();
                    }

                    if (auto rbComp = (*componentTable)["_rigidBodyComponent"])
                    {
                        auto entityRb = entityRef.AddComponent<RigidBodyComponent>(entityRef).GetRigidBody();

                        if(auto mass = rbComp["_mass"].value<float>())
                            entityRb->SetMass(*mass);
                        else WARN_NO_VALUE("RigidBodyComp: _mass doesnt exist for entity: {}", *tagComponent);
                        if(auto damping = rbComp["_damping"].value<float>())
                            entityRb->SetDamping(*damping);
                        else WARN_NO_VALUE("RigidBodyComp: _damping doesnt exist for entity: {}", *tagComponent);
                        if(auto angularDamping = rbComp["_angularDamping"].value<float>())
                            entityRb->SetAngularDamping(*angularDamping);
                        else WARN_NO_VALUE("RigidBodyComp: _angularDamping doesnt exist for entity: {}", *tagComponent);

                        if(auto bodyType = rbComp["_bodyType"].value<int>())
                            entityRb->SetBodyType(static_cast<BodyType>(*bodyType));
                        else WARN_NO_VALUE("RigidBodyComp: _bodyType doesnt exist for entity: {}", *tagComponent);
                        if(auto active = rbComp["_active"].value<bool>())
                            entityRb->SetActive(*active);
                        else WARN_NO_VALUE("RigidBodyComp: _active doesnt exist for entity: {}", *tagComponent);
                        if(auto useAutoMass = rbComp["_useAutoMass"].value<bool>())
                            entityRb->SetAutoMass(*useAutoMass);
                        else WARN_NO_VALUE("RigidBodyComp: _useAutoMass doesnt exist for entity: {}", *tagComponent);
                        if(auto useAutoCenterOfMass = rbComp["_useAutoCenterOfMass"].value<bool>())
                            entityRb->SetAutoCenterOfMass(*useAutoCenterOfMass);
                        else WARN_NO_VALUE("RigidBodyComp: _useAutoCenterOfMass doesnt exist for entity: {}", *tagComponent);
                        if(auto useGravity = rbComp["_useGravity"].value<bool>())
                            entityRb->SetUseGravity(*useGravity);
                        else WARN_NO_VALUE("RigidBodyComp: _useGravity doesnt exist for entity: {}", *tagComponent);
                    }

                    if (auto colliderComp = (*componentTable)["_colliderComponent"])
                    {
                        auto& entityColliderComp = entityRef.AddComponent<ColliderComponent>(entityRef);
                        auto collidersView = colliderComp["_colliders"];
                        if (toml::array* colliders = collidersView.as_array())
                        {
                            for (auto& collider : *colliders)
                            {
                                toml::table* colliderTable = collider.as_table();
                                if (!colliderTable) continue;

                                Collider* col = nullptr;
                                auto specificData = (*colliderTable)["_colliderSpecificData"];

                                switch ((*colliderTable)["_shapeType"].value_or<int>(-1)) //If no value exists fall outside of case range
                                {
                                    case 0:
                                    {
                                        if(auto radius = specificData["_radius"].value<float>())
                                            col = entityColliderComp.AddSphereCollider(*radius);
                                        else WARN_NO_VALUE("SphereCollider: _radius of doesnt exist for entity: {}", *tagComponent);
                                    }
                                    break;
                                    case 1:
                                    {
                                        if(auto halfSize = specificData["_halfSize"])
                                        {
                                            Vector3 halfExtends = Vector3(
                                            *halfSize[0].value<float>(),
                                            *halfSize[1].value<float>(),
                                            *halfSize[2].value<float>());
                                            col = entityColliderComp.AddBoxCollider(halfExtends);
                                        }
                                        else WARN_NO_VALUE("BoxCollider: _halfSize doesnt exist for entity: {}", *tagComponent);
                                    }
                                    break;
                                    case 2:
                                    {
                                        if(auto radius = specificData["_radius"].value<float>(); auto height = specificData["_height"].value<float>())
                                            col = entityColliderComp.AddCapsuleCollider(*radius, *height);
                                        else WARN_NO_VALUE("CapsuleCollider: _radius or _height for CapsuleCollider doesnt exist for entity: {}", *tagComponent);
                                    }
                                    break;
                                    default: WARN_NO_VALUE("Collider: _shapeType out of range for entity {}", *tagComponent);
                                }
                                if (col != nullptr)
                                {
                                    if(auto colPos = (*colliderTable)["_colliderPosition"])
                                        col->SetPosition(Vector3(*colPos[0].value<float>(), *colPos[1].value<float>(), *colPos[2].value<float>()));
                                    else WARN_NO_VALUE("Collider: _colliderPosition doesnt exist for entity {}", *tagComponent);
                                    if(auto colRot = (*colliderTable)["_colliderRotation"])
                                        col->SetRotation(Vector3(*colRot[0].value<float>(), *colRot[1].value<float>(), *colRot[2].value<float>()), true);
                                    else WARN_NO_VALUE("Collider: _colliderRotation doesnt exist for entity {}", *tagComponent);

                                    if(auto colMask = (*colliderTable)["_colliderCollisionMask"].value<int>())
                                        col->SetCollisionMask(*colMask);
                                    else WARN_NO_VALUE("Collider: _colliderCollisionMask doesnt exist for entity {}", *tagComponent);
                                    if(auto colCategories = (*colliderTable)["_colliderCollisionCategories"].value<int>())
                                        col->SetCollisionCategories(*colCategories);
                                    else WARN_NO_VALUE("Collider: _colliderCollisionCategories doesnt exist for entity {}", *tagComponent);

                                    if(auto colMaterial = (*colliderTable)["_colliderMaterial"])
                                    {
                                        col->GetMaterial().setFrictionCoefficient(*colMaterial[0].value<float>());
                                        col->GetMaterial().setBounciness(*colMaterial[1].value<float>());
                                        col->GetMaterial().setMassDensity(*colMaterial[2].value<float>());
                                    }
                                    else WARN_NO_VALUE("Collider: _colliderMaterial doesnt exist for entity {}", *tagComponent);
                                }
                            }
                        }
                    }

                    if (auto renderComponent = (*componentTable)["_renderComponent"]; renderComponent)
                    {
                        if(auto modelPath = renderComponent["_modelPath"].value<std::string>())
                            entityRef.AddComponent<RenderComponent>(*modelPath);
                        else WARN_NO_VALUE("RenderComp: _colliderMaterial doesnt exist for entity {}", *tagComponent);
                    }

                    if(auto lightComp = (*componentTable)["_lightComponent"]; lightComp)
                    {
                        auto& entityLightComponent = entityRef.AddComponent<LightComponent>();
                        if(auto colorView = lightComp["_color"])
                        {
                            Vector3 color = Vector3(
                            *colorView[0].value<float>(),
                            *colorView[1].value<float>(),
                            *colorView[2].value<float>());

                            entityLightComponent.SetColor(color);
                        }
                        else WARN_NO_VALUE("LightComp: _color doesnt exist for entity: {}", *tagComponent);

                        if(auto intensity = lightComp["_intensity"].value<float>())
                            entityLightComponent.SetIntensity(*intensity);
                        else WARN_NO_VALUE("LightComp: _intensity doesnt exist for entity {}", *tagComponent);
                        if(auto lightType = lightComp["_lightType"].value<int>())
                            entityLightComponent.SetLightType(static_cast<LightType>(*lightType));
                        else WARN_NO_VALUE("LightComp: _lightType doesnt exist for entity {}", *tagComponent);
                    }

                    // ScriptComponent
                }
            }
        }

        ANE_ELOG_INFO("End Deserializing file: {}", key);

        return scene;
    }

    std::shared_ptr<Scene> SceneSerializer::DeserializeBinary()
    {
        ANE_PROFILE_FUNCTION();

        // not implemented
        ANE_ASSERT(false);

        Scene scene{"key"};

        return std::make_shared<Scene>("key");
    }

    void SceneSerializer::WriteToFile(std::string filepath, const toml::table& content)
    {
        ANE_PROFILE_FUNCTION();

        std::ofstream output(filepath, std::ios::trunc);

        if (!output.is_open()) output.open(filepath);

        output << content << "\n\n";

        output.close();
    }

    void SceneSerializer::WriteToFile(std::string filepath)
    {
        ANE_PROFILE_FUNCTION();

        std::ofstream output(filepath, std::ios::app);

        if (!output.is_open()) output.open(filepath);

        output.close();
    }
}
