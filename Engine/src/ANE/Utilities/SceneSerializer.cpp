﻿#include "anepch.h"
#include "SceneSerializer.h"

#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Layers/Layer.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Core/Scene/Components/ColliderComponent.h"
#include "ANE/Core/Scene/Components/NativeScriptComponent.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "ANE/Core/Scene/Components/RigidBodyComponent.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Physics/PhysicsTypes.h"
#include "ANE/Physics/Types/BoxCollider.h"
#include "ANE/Physics/Types/CapsuleCollider.h"
#include "ANE/Physics/Types/SphereCollider.h"


namespace Engine
{
    SceneSerializer::SceneSerializer()
    {
    }

    toml::table SceneSerializer::EntitySerializer(Entity& entity)
    {
        toml::table componentArray;

        if (UUIDComponent uuidComponent; entity.TryGetComponent(uuidComponent))
        {
            componentArray.insert_or_assign("_uuid", uuidComponent.UUID);
        }
        //
        // if (TagComponent tagComp; entity.TryGetComponent(tagComp))
        // {
        //     componentArray.insert_or_assign("_tag", tagComp.Value);
        // }

        if (TransformComponent transformComp; entity.TryGetComponent(transformComp))
        {
            auto table = toml::table{{
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

            componentArray.insert_or_assign("_transformComponent", table);
        }

        if (CameraComponent cameraComp; entity.TryGetComponent(cameraComp))
         {
             auto table = toml::table{
                 {"_aspectRatio", cameraComp.GetAspectRatio()},
                 {"_fieldOfView", cameraComp.GetFOV()},
                 {"zNear", cameraComp.GetNearClip()},
                 {"zFar", cameraComp.GetFarClip()}
             };
             componentArray.insert_or_assign("_cameraComponent", table);
         }


         //Not completed
        if (ColliderComponent colliderComp; entity.TryGetComponent(colliderComp))
         {
             toml::array colliders;
             for (auto collider : colliderComp.GetColliders())
             {
                 toml::array payload;
                 switch (collider->GetShapeType())
                 {
                     case CollisionShapeType::Box:
                     {
                         auto castedBoxCollider = reinterpret_cast<BoxCollider*>(collider);
                         payload.emplace_back<toml::array>("_boxCollider", toml::array{
                                                               "_halfSize", toml::array{
                                                                   castedBoxCollider->GetHalfSize().X,
                                                                   castedBoxCollider->GetHalfSize().Y,
                                                                   castedBoxCollider->GetHalfSize().Z
                                                               }
                                                           });
                         break;
                     }
                     case CollisionShapeType::Sphere:
                     {
                         auto castedSphereCollider = reinterpret_cast<SphereCollider*>(collider);
                         payload.emplace_back<toml::array>("_sphereCollider", toml::array{"_radius", castedSphereCollider->GetRadius()});
                         break;
                     }
                     case CollisionShapeType::Capsule:
                     {
                         auto castedCapsuleCollider = reinterpret_cast<CapsuleCollider*>(collider);
                         payload.emplace_back<toml::array>("_capsuleCollider", toml::array{toml::array{"_radius", castedCapsuleCollider->GetRadius()}},
                                                           toml::array{"_height", castedCapsuleCollider->GetHeight()});
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
                             collider->GetMaterial().getFrictionCoefficient(), // squared?
                             collider->GetMaterial().getBounciness(),
                             collider->GetMaterial().getMassDensity()
                         }
                     },
                     {"_colliderCollisionMask", collider->GetCollisionMask()},
                     {"_shapeType", collider->GetShapeType()},
                     {"_colliderSpecificData", payload}
                 };

                 colliders.emplace_back(table);
             }

             auto table = toml::table{
                 {"_colliders", colliders}
             };
             componentArray.insert_or_assign("_colliderComponent", table);
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

        if (RenderComponent renderComp; entity.TryGetComponent(renderComp))
         {
             auto table = toml::table{
                 {"_modelPath", renderComp.GetModelPath()}
             };

             componentArray.insert_or_assign("_renderComponent", table);
         }

        if (RigidBodyComponent rigidComp; entity.TryGetComponent(rigidComp))
         {
             auto table = toml::table{
                 {"_bodyType", rigidComp.GetRigidBody()->GetBodyType()},
                 {"_useGravity", rigidComp.GetRigidBody()->IsGravityEnabled()},
                 {"_mass", rigidComp.GetRigidBody()->GetMass()},
                 {"_active", rigidComp.GetRigidBody()->IsActive()}
             };
             componentArray.insert_or_assign("_rigidBodyComponent", table);
         }


         // UUIDComponent UUIDComp;
         // if (entity.TryGetComponent(UUIDComp))
         // {
         //     toml::table table{
         //         {"UUID", UUIDComp.UUID}
         //     };
         //     componentArray.emplace_back(table);
         // }

        return componentArray;
    }

    void SceneSerializer::Serialize(const std::shared_ptr<Scene>& scene)
    {
        toml::table arr;
        // auto path = _filepath << _scene.

        for (auto entity : scene->_registry.view<TagComponent>())
        {
            Entity ent = {entity, scene.get()};

            arr.insert_or_assign(ent.GetComponent<TagComponent>().Value, EntitySerializer(ent));
        }

        // toml::array tables;
        // toml::array data1{3, 2
        // };
        // toml::array data2{3, 2
        // };
        // auto TestTable = toml::table{
        //     {"ID", "value"},
        //     {"ID2", data1}
        // };
        //
        // auto TestTable2 = toml::table{
        //     {"ID3", "value2"},
        //     {"ID24", data2}
        // };
        //
        // tables.push_back(TestTable);
        // tables.push_back(TestTable2);


         auto table = toml::table{
             {"Scene", scene->Name},
             {"Entities", arr}
         };

        std::string path = _filepath + scene->Name + ".toml";

        WriteToFile(path, table);


        //ANE_ELOG(table);
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

    std::shared_ptr<Scene> SceneSerializer::Deserialize(const char* key, EditorLayer* layer) const // todo: create scene should be moved into layer maybe?
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

        const auto sceneName = table["Scene"].value_or("NoSceneFound"); // dont know if sv is correct here
        ANE_ELOG(sceneName);
        auto scene = std::make_shared<Scene>(sceneName);

        if (!table.empty())
        {
            auto nodeView = table["Entities"];
            if (toml::array* entityArray = nodeView.as_array())
            {
                for (auto& entity : *entityArray)
                {
                    if constexpr (toml::is_array<decltype(entity)>)
                    {
                        ANE_ELOG("We are an array");
                    }
                    else if constexpr (toml::is_table<decltype(entity)>)
                    {
                        ANE_ELOG("We are a table");
                    }

                    //     ANE_ELOG("We have type of: {}", entity.type());
                    // if (const auto& entityTable = entity.as_table())
                    // {
                    //
                    //     if (auto tag = (*entityTable)["_tag"].value<std::string>())
                    //     {
                    //         scene->Create(*tag);
                    //     }
                    // }
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
