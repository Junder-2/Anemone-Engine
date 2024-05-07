#include "anepch.h"
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

    toml::array SceneSerializer::EntitySerializer(Entity& entity)
    {
        toml::array componentArray;

        TagComponent tagComp;
        if (entity.TryGetComponent(tagComp))
        {
            toml::table table{
                {"_tag", tagComp.Value}
            };
            componentArray.emplace_back(table);
        }

        TransformComponent transformComp;
        if (entity.TryGetComponent(transformComp))
        {
            toml::table table{
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
            componentArray.emplace_back(table);
        }

        CameraComponent cameraComp;
        if (entity.TryGetComponent(cameraComp))
        {
            toml::table table{
                {"_aspectRatio", cameraComp.GetAspectRatio()},
                {"_fieldOfView", cameraComp.GetFOV()},
                {"zNear", cameraComp.GetNearClip()},
                {"zFar", cameraComp.GetFarClip()}
            };
            componentArray.emplace_back(table);
        }


        //Not completed
        ColliderComponent colliderComp;
        if (entity.TryGetComponent(colliderComp))
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

                toml::table table{
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

            toml::table table{
                {"_colliders", colliders}
            };
            componentArray.emplace_back(table);
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

        RenderComponent renderComp;
        if (entity.TryGetComponent(renderComp))
        {
            toml::table table{
                {"_modelPath", renderComp.GetModelPath()}
            };

            componentArray.emplace_back(table);
        }

        RigidBodyComponent rigidComp;
        if (entity.TryGetComponent(rigidComp))
        {
            toml::table table{
                {"_bodyType", rigidComp.GetRigidBody()->GetBodyType()},
                {"_useGravity", rigidComp.GetRigidBody()->IsGravityEnabled()},
                {"_mass", rigidComp.GetRigidBody()->GetMass()},
                {"_active", rigidComp.GetRigidBody()->IsActive()}
            };
            componentArray.emplace_back(table);
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
        toml::array arr;
        // auto path = _filepath << _scene.

        for (auto entity : scene->_registry.view<TagComponent>())
        {
            Entity ent = {entity, scene.get()};


            arr.push_back(EntitySerializer(ent));
        }


        const toml::table table{
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

    std::shared_ptr<Scene> SceneSerializer::Deserialize(const char* key, EditorLayer* layer) const // todo: create scene should be moved into layer maybe?
    {
        std::string filePath = _filepath + key + ".toml";
        toml::table table = toml::parse_file(filePath);

        const auto sceneName = table["Scene"].value_or("sv"); // dont know if sv is correct here
        auto scene = std::make_shared<Scene>(sceneName);

        auto nodeView = table["Entities"];
        if (toml::array* entityArray = nodeView.as_array())
        {
            entityArray->for_each([=](auto&& element)
            {

                //toml::array* componentsArray = element[]

                //auto tag = tagTable["_tag"].value_or("sv"); // dont know if sv is correct here

               // scene.get()->Create(tag);


                //layer->Create()



            });
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
        std::ofstream output(filepath, std::ios::app);

        if (!output.is_open()) output.open(filepath);

        output << content << "\n\n";

        output.close();
    }
}
