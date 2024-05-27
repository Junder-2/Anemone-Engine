#pragma once
#include <imgui.h>

#include "AneImGui.h"
#include "PhysicsUtilities.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "ANE/Math/FMath.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/Types/Collider.h"
#include "ANE/Physics/Types/BoxCollider.h"
#include "ANE/Physics/Types/SphereCollider.h"
#include "ANE/Physics/Types/CapsuleCollider.h"
#include "ANE/Physics/Types/RigidBody.h"
#include "ANE/Renderer/Mesh.h"

namespace Engine
{
    inline bool InspectMutableStringField(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<std::string>();
        bool propertyWritten = false;
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());

        const auto label = field.prop("display_name"_hs).value().cast<const char*>();

        if(AneImGui::LabelInputText(label, buffer, sizeof(buffer)))
        {
            propertyWritten = true;
        }

        if(propertyWritten) field.set(componentData, std::string(buffer));
        return propertyWritten;
    }
    inline bool InspectMutableTextureField(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<std::string>();
        bool propertyWritten = false;
        bool writable = field.prop(EDITABLE_HASH).value().cast<bool>();
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());

        const auto label = field.prop("display_name"_hs).value().cast<const char*>();

        if(AneImGui::LabelInputText(label, buffer, sizeof(buffer)))
        {
            propertyWritten = true;
        }

        if(propertyWritten) field.set(componentData, std::string(buffer));
        return propertyWritten;
    }

    inline bool InspectMutableBoolField(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<bool>();
        bool propertyWritten = false;
        const auto label = field.prop("display_name"_hs).value().cast<const char*>();

        if(AneImGui::LabelCheckbox(label, &v))
        {
            propertyWritten = true;
        }

        if(propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableTransformMatrix(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<TransformMatrix>();
        bool propertyWritten = false;
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);

        if (AneImGui::LabelDragFloat3("Position", &position.X, 0.1f))
        {
            v.SetPosition(position);
            propertyWritten = true;
        }

        if (AneImGui::LabelDragFloat3("Rotation", &rotation.X, 0.1f))
        {
            v.SetRotation(rotation, true);
            propertyWritten = true;
        }

        if (AneImGui::LabelDragFloat3("Scale", &scale.X, 0.1f))
        {
            v.SetScale(scale);
            propertyWritten = true;
        }

        if(propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableColliders(entt::meta_data& field, entt::meta_any& componentData)
    {
        bool propertyWritten = false;
        auto data = field.get(componentData).cast<ColliderComponentData>();
        ImGui::Text("%s", field.prop("display_name"_hs).value().cast<const char*>());
        int index = 0;
        for (const auto collider : data.Colliders)
        {
            index++;
            constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            std::string label = PhysicsUtilities::ToString(collider->GetShapeType()).append(std::format("Collider {}", index));
            if (ImGui::TreeNodeEx(label.c_str(), nodeFlags))
            {
                Vector3 position = collider->GetPosition();
                Vector3 rotation = collider->GetEulerAngles(true);
                if (AneImGui::LabelDragFloat3("Local Position", &position.X, 0.1f))
                {
                    collider->SetPosition(position);
                    propertyWritten = true;
                }
                if (AneImGui::LabelDragFloat3("Local Rotation", &rotation.X, 0.1f))
                {
                    collider->SetRotation(rotation, true);
                    propertyWritten = true;
                }

                switch (collider->GetShapeType())
                {
                    case CollisionShapeType::Sphere:
                    {
                        const auto sphereCollider = reinterpret_cast<SphereCollider*>(collider);
                        float radius = sphereCollider->GetRadius();
                        if (AneImGui::LabelDragFloat("Radius", &radius, 0.1f, FLT_MIN, FLT_MAX))
                        {
                            sphereCollider->SetRadius(radius);
                            propertyWritten = true;
                        }
                    }
                    break;
                    case CollisionShapeType::Box:
                    {
                        const auto boxCollider = reinterpret_cast<BoxCollider*>(collider);
                        Vector3 halfSize = boxCollider->GetHalfSize();
                        if (AneImGui::LabelDragFloat3("Half Size", &halfSize.X, 0.1f, FLT_MIN, FLT_MAX))
                        {
                            boxCollider->SetHalfSize(halfSize);
                            propertyWritten = true;
                        }
                    }
                    break;
                    case CollisionShapeType::Capsule:
                    {
                        const auto capsuleCollider = reinterpret_cast<CapsuleCollider*>(collider);
                        float radius = capsuleCollider->GetRadius();
                        float height = capsuleCollider->GetHeight();
                        if (AneImGui::LabelDragFloat("Radius", &radius, 0.1f, FLT_MIN, FLT_MAX))
                        {
                            capsuleCollider->SetRadius(radius);
                            propertyWritten = true;
                        }
                        if (AneImGui::LabelDragFloat("Height", &height, 0.1f, FLT_MIN, FLT_MAX))
                        {
                            capsuleCollider->SetHeight(height);
                            propertyWritten = true;
                        }
                    }
                    break;
                }
                ImGui::TreePop();
            }
        }

        if(ImGui::Button("Add SphereCollider"))
        {
            if(Entity ownerEntity = GetPhysicsSystem().GetOwnerEntity(data.BodyId); ownerEntity)
            {
                ownerEntity.GetComponent<ColliderComponent>().AddSphereCollider(1.f);
            }
        }
        if(ImGui::Button("Add BoxCollider"))
        {
            if(Entity ownerEntity = GetPhysicsSystem().GetOwnerEntity(data.BodyId); ownerEntity)
            {
                ownerEntity.GetComponent<ColliderComponent>().AddBoxCollider(1.f);
            }
        }
        if(ImGui::Button("Add CapsuleCollider"))
        {
            if(Entity ownerEntity = GetPhysicsSystem().GetOwnerEntity(data.BodyId); ownerEntity)
            {
                ownerEntity.GetComponent<ColliderComponent>().AddCapsuleCollider(1.f, 2.f);
            }
        }

        if (propertyWritten) field.set(componentData, data);
        return propertyWritten;
    }

    inline bool InspectMutableRigidBody(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto rb = field.get(componentData).cast<RigidBody*>();
        bool propertyWritten = false;

        bool isActive = rb->IsActive();
        int bodyType = ToInt(rb->GetBodyType());
        bool isGravity = rb->IsGravityEnabled();
        bool isAutoMass = rb->IsAutoMass();
        bool isAutoCenterOfMass = rb->IsAutoCenterOfMass();
        float mass = rb->GetMass();
        float damping = rb->GetDamping();
        float angularDamping = rb->GetAngularDamping();

        const char* bodyTypes[3] {"Static", "Kinematic", "Dynamic"};

        if (AneImGui::LabelCheckbox("Active", &isActive))
        {
            rb->SetActive(isActive);
            propertyWritten = true;
        }
        if (AneImGui::LabelCombo("BodyType", &bodyType, bodyTypes, 3))
        {
            rb->SetBodyType(static_cast<BodyType>(bodyType));
            propertyWritten = true;
        }
        if (AneImGui::LabelCheckbox("Use Gravity", &isGravity))
        {
            rb->SetUseGravity(isGravity);
            propertyWritten = true;
        }
        if (AneImGui::LabelCheckbox("Use Auto Center of Mass", &isAutoCenterOfMass))
        {
            rb->SetAutoCenterOfMass(isAutoCenterOfMass);
            propertyWritten = true;
        }
        if (AneImGui::LabelCheckbox("Use Auto Mass", &isAutoMass))
        {
            rb->SetAutoMass(isAutoMass);
            propertyWritten = true;
        }
        ImGui::BeginDisabled(isAutoMass);
        if (AneImGui::LabelDragFloat("Mass", &mass, .1f, 0, FLT_MAX))
        {
            rb->SetMass(mass);
            propertyWritten = true;
        }
        ImGui::EndDisabled();
        if (AneImGui::LabelDragFloat("Damping", &damping, .1f, 0, FLT_MAX))
        {
            rb->SetDamping(damping);
            propertyWritten = true;
        }
        if (AneImGui::LabelDragFloat("Angular Damping", &angularDamping, .1f, 0, FLT_MAX))
        {
            rb->SetAngularDamping(angularDamping);
            propertyWritten = true;
        }

        constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (ImGui::TreeNodeEx("Readonly", nodeFlags))
        {
            bool isSleeping = rb->IsSleeping();
            Vector3 position = rb->GetPosition();
            Vector3 rotation = rb->GetRotation().GetEulerAngles(true);
            Vector3 velocity = rb->GetVelocity();
            Vector3 angularVelocity = rb->GetAngularVelocity();

            ImGui::BeginDisabled();
            AneImGui::LabelCheckbox("Sleeping", &isSleeping);
            ImGui::Spacing();
            AneImGui::LabelDragFloat3("Position", &position.X);
            AneImGui::LabelDragFloat3("Rotation", &rotation.X);
            ImGui::Spacing();
            AneImGui::LabelDragFloat3("Velocity", &velocity.X);
            AneImGui::LabelDragFloat3("Angular Velocity", &angularVelocity.X);
            ImGui::EndDisabled();

            ImGui::TreePop();
        }

        if (propertyWritten) field.set(componentData, rb);
        return propertyWritten;
    }

    inline bool InspectMutableFloat(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<float>();
        bool propertyWritten = false;

        if (AneImGui::LabelDragFloat(field.prop("display_name"_hs).value().cast<char const*>(), &v, 0.1f))
        {
            propertyWritten = true;
        }

        if (propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableMeshAsset(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<VmaMeshAsset*>();
        bool propertyWritten = false;

        //AneImGui::LabelText(field.prop("display_name"_hs).value().cast<const char*>(), v.Name.c_str());
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v->Name.c_str());
        if(AneImGui::LabelInputText(field.prop("display_name"_hs).value().cast<const char*>(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            VmaMeshAsset* newMesh = Renderer::LoadModel(buffer);
            if (newMesh != nullptr)
            {
                v = newMesh;
                propertyWritten = true;
            }
            else
            {
                ANE_ELOG_WARN("Couldn't find mesh: {}", buffer);
            }
        }

        if (propertyWritten)
        {
            field.set(componentData, v);
        }

        return propertyWritten;
    }

    inline bool InspectMutableVector2Field(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<Vector2>();
        bool propertyWritten = false;

        if (AneImGui::LabelDragFloat2(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = true;
        }

        if (propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableVector3Field(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<Vector3>();
        bool propertyWritten = false;

        if (AneImGui::LabelDragFloat3(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = true;
        }

        if (propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableVector4Field(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<Vector4>();
        bool propertyWritten = false;

        if (AneImGui::LabelDragFloat4(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = true;
        }

        if (propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableMaterialData(entt::meta_data& field, entt::meta_any& componentData)
    {
        MaterialData matData = field.get(componentData).cast<MaterialData>();
        auto& mpb = *matData.Material->Uniforms;

        bool propertyWritten = false;

        if(AneImGui::LabelColorEdit3("Base Color", &mpb.Color.R))
        {
            propertyWritten =  true;
        }

        if(AneImGui::LabelColorEdit3("Emission Color", &mpb.Emission.R))
        {
            propertyWritten =  true;
        }

        if(AneImGui::LabelSliderFloat("Height", &mpb.Height, 0, 1))
        {
            propertyWritten =  true;
        }

        if(AneImGui::LabelSliderFloat("Metallic", &mpb.Metallic, 0, 1))
        {
            propertyWritten =  true;
        }

        if(AneImGui::LabelSliderFloat("Normal", &mpb.Normal, 0, 1))
        {
            propertyWritten = true;
        }

        if(AneImGui::LabelSliderFloat("Occlusion", &mpb.Occlusion, 0, 1))
        {
            propertyWritten =  true;
        }

        if(AneImGui::LabelSliderFloat("Reflectance", &mpb.Reflectance, 0, 1))
        {
            propertyWritten =  true;
        }

        if(AneImGui::LabelSliderFloat("Roughness", &mpb.Roughness, 0, 1))
        {
            propertyWritten =  true;
        }

        ImGui::BeginDisabled(matData.HasUniqueMaterial);
        if (ImGui::Button("Create Unique Material"))
        {
            matData.Material = Renderer::GetDefaultMaterialClone();
            matData.HasUniqueMaterial = true;
            propertyWritten = true;
        }
        ImGui::EndDisabled();

        if(propertyWritten) field.set(componentData, matData);
        return propertyWritten;
    }

    inline bool InspectMutableLightData(entt::meta_data& field, entt::meta_any& componentData)
    {
        LightData lightData = field.get(componentData).cast<LightData>();

        bool propertyWritten = false;

        if(AneImGui::LabelColorEdit3("Light Color", &lightData.Color.R))
        {
            propertyWritten = true;
        }

        if(AneImGui::LabelDragFloat("Intensity", &lightData.Intensity))
        {
            propertyWritten = true;
        }

        //Todo: LightType

        if(propertyWritten) field.set(componentData, lightData);
        return propertyWritten;
    }

    using FieldInspectorFn = bool (*)(entt::meta_data& field, entt::meta_any& component_data);

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> g_mutable_data_inspectors
    {
        {entt::type_id<std::string>().hash(), InspectMutableStringField},
        {entt::type_id<TransformMatrix>().hash(), InspectMutableTransformMatrix},
        {entt::type_id<ColliderComponentData>().hash(), InspectMutableColliders},
        {entt::type_id<RigidBody*>().hash(), InspectMutableRigidBody},
        {entt::type_id<VmaMeshAsset*>().hash(), InspectMutableMeshAsset},
        {entt::type_id<float>().hash(), InspectMutableFloat},
        {entt::type_id<Vector2>().hash(), InspectMutableVector2Field},
        {entt::type_id<Vector3>().hash(), InspectMutableVector3Field},
        {entt::type_id<Vector4>().hash(), InspectMutableVector4Field},
        {entt::type_id<bool>().hash(), InspectMutableBoolField},
        {entt::type_id<MaterialData>().hash(), InspectMutableMaterialData},
        {entt::type_id<LightData>().hash(), InspectMutableLightData}
    };

    inline bool InspectImmutableStringField(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<std::string>();

        AneImGui::LabelText(field.prop("display_name"_hs).value().cast<const char*>(), v.c_str());

        return true;
    }

    inline bool InspectImmutableTransformMatrix(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<TransformMatrix>();
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);

        ImGui::Text("Read Only");

        ImGui::BeginDisabled();
        AneImGui::LabelDragFloat3("Position", &position.X);
        AneImGui::LabelDragFloat3("Rotation", &rotation.X);
        AneImGui::LabelDragFloat3("Scale", &scale.X);
        ImGui::EndDisabled();

        return true;
    }

    inline bool InspectImmutableMeshAsset(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<VmaMeshAsset>();

        AneImGui::LabelText(field.prop("display_name"_hs).value().cast<const char*>(), v.Name.c_str());

        return true;
    }

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> _immutableDataInspectors
    {
        {entt::type_id<std::string>().hash(), InspectImmutableStringField},
        {entt::type_id<TransformMatrix>().hash(), InspectImmutableTransformMatrix},

         {entt::type_id<VmaMeshAsset>().hash(), InspectImmutableMeshAsset}
          /*,
         {entt::type_id<float>().hash(), inspect_float},
         {entt::type_id<Vector2>().hash(), inspect_vector2_field},
         {entt::type_id<Vector3>().hash(), inspect_vector3_field},
         {entt::type_id<Vector4>().hash(), inspect_vector4_field}
         */
    };

    template <typename ...Args>
    inline auto InvokeMetaFunction(entt::meta_type meta, const entt::id_type func_id, Args&&...args)
    {
        if (!meta)
        {
            ANE_LOG_ERROR("No relevant meta type registered");
        }
        if (auto meta_function = meta.func(func_id); meta_function)
        {
            return meta_function.invoke({}, std::forward<Args>(args)...);
        }
        return entt::meta_any{};
    }

    template <typename ...Args>
    inline auto InvokeMetaFunction(entt::id_type id, entt::id_type funcId, Args&&...args)
    {
        return InvokeMetaFunction(entt::resolve(id), funcId, std::forward<Args>(args)...);
    }

    /*
     * Mesh - VMA Mesh asset
     * String - File
     * Material Property - two floats, one int, a blah blah
     *
     *
     *
     */
}
