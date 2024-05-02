#pragma once
#include <imgui.h>
#include "PhysicsUtilities.h"
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
        bool writable = field.prop(EDITABLEHASH).value().cast<bool>();
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s",field.prop("display_name"_hs).value().cast<const char*>());
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        if (ImGui::InputText("##stringfield", buffer, sizeof(buffer)))
        {
            propertyWritten = field.set(componentData, std::string(buffer));
        }
        return propertyWritten;
    }

    inline bool InspectMutableTransformMatrix(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<TransformMatrix>();
        bool propertyWritten = false;
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);
        ImGui::Text("%s", field.prop("display_name"_hs).value().cast<const char*>());
        if (ImGui::DragFloat3(field.prop("Position"_hs).value().cast<char const*>(), &position.X, 0.1f))
        {
            v.SetPosition(position);
            propertyWritten = field.set(componentData, v);
        }
        if (ImGui::DragFloat3(field.prop("Rotation"_hs).value().cast<char const*>(), &rotation.X, 0.1f))
        {
            v.SetRotation(rotation, true);
            propertyWritten = field.set(componentData, v);
        }
        if (ImGui::DragFloat3(field.prop("Scale"_hs).value().cast<char const*>(), &scale.X, 0.1f))
        {
            v.SetScale(scale);
            propertyWritten = field.set(componentData, v);
        }
        return propertyWritten;
    }

    inline bool InspectMutableColliders(entt::meta_data& field, entt::meta_any& componentData)
    {
        bool propertyWritten = false;
        auto v = field.get(componentData).cast<std::vector<Collider*>>();
        ImGui::Text("%s", field.prop("display_name"_hs).value().cast<const char*>());
        for (auto collider : v)
        {
            constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            if (ImGui::TreeNodeEx(PhysicsUtilities::ToString(collider->GetShapeType()).append(" Collider").c_str(), nodeFlags))
            {
                Vector3 position = collider->GetPosition();
                Vector3 rotation = collider->GetEulerAngles(true);
                if (ImGui::DragFloat3("Local Position", &position.X, 0.1f))
                {
                    collider->SetPosition(position);
                    propertyWritten = true;
                }
                if (ImGui::DragFloat3("Local Rotation", &rotation.X, 0.1f))
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
                        if (ImGui::DragFloat("Radius", &radius, 0.1f))
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
                        if (ImGui::DragFloat3("Half Size", &halfSize.X, 0.1f))
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
                        if (ImGui::DragFloat("Radius", &radius, 0.1f))
                        {
                            capsuleCollider->SetRadius(radius);
                            propertyWritten = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::DragFloat("Height", &height, 0.1f))
                        {
                            capsuleCollider->SetHeight(radius);
                            propertyWritten = true;
                        }
                    }
                    break;
                }
                ImGui::TreePop();
            }
        }
        if (propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableRigidBody(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<RigidBody*>();
        bool propertyWritten = false;

        constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (ImGui::TreeNodeEx("Readonly", nodeFlags))
        {
            Vector3 position = v->GetPosition();
            Vector3 rotation = v->GetRotation().GetEulerAngles(true);
            Vector3 velocity = v->GetVelocity();
            Vector3 angularVelocity = v->GetAngularVelocity();
            bool isSleeping = v->IsSleeping();
            bool isActive = v->IsActive();

            ImGui::BeginDisabled();
            ImGui::DragFloat3("Position", &position.X);
            ImGui::DragFloat3("Rotation", &rotation.X);
            ImGui::Spacing();
            ImGui::DragFloat3("Velocity", &velocity.X);
            ImGui::DragFloat3("Angular Velocity", &angularVelocity.X);
            ImGui::Spacing();
            ImGui::Checkbox("Active", &isActive);
            ImGui::SameLine();
            ImGui::Checkbox("Sleeping", &isSleeping);
            ImGui::EndDisabled();

            ImGui::TreePop();
        }

        if (propertyWritten) field.set(componentData, v);
        return propertyWritten;
    }

    inline bool InspectMutableFloat(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<float>();
        bool propertyWritten = false;

        if (ImGui::DragFloat(field.prop("display_name"_hs).value().cast<char const*>(), &v, 0.1f))
        {
            propertyWritten = field.set(component_data, v); //insufficient for dirty flag!!!
        }
        return propertyWritten;
        //ImGui::Text("%d",v);
    }

    inline bool InspectMutableMeshAsset(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<VmaMeshAsset>();
        bool propertyWritten = false;

        ImGui::Text("%s", v.Name.c_str());
        return propertyWritten;
    }

    inline bool InspectMutableVector2Field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<Vector2>();
        bool propertyWritten = false;

        if (ImGui::DragFloat2(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = field.set(component_data, v);
        }
        return propertyWritten;
    }

    inline bool InspectMutableVector3Field(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<Vector3>();
        bool propertyWritten = false;

        if (ImGui::DragFloat3(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = field.set(componentData, v);
        }
        return propertyWritten;
    }

    inline bool InspectMutableVector4Field(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<Vector4>();
        bool propertyWritten = false;

        if (ImGui::DragFloat4(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = field.set(componentData, v);
        }
        return propertyWritten;
    }

    using FieldInspectorFn = bool (*)(entt::meta_data& field, entt::meta_any& component_data);

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> g_mutable_data_inspectors
    {
        {entt::type_id<std::string>().hash(), InspectMutableStringField},
        {entt::type_id<TransformMatrix>().hash(), InspectMutableTransformMatrix},
        {entt::type_id<std::vector<Collider*>>().hash(), InspectMutableColliders},
        {entt::type_id<RigidBody*>().hash(), InspectMutableRigidBody},
        {entt::type_id<VmaMeshAsset>().hash(), InspectMutableMeshAsset},
        {entt::type_id<float>().hash(), InspectMutableFloat},
        {entt::type_id<Vector2>().hash(), InspectMutableVector2Field},
        {entt::type_id<Vector3>().hash(), InspectMutableVector3Field},
        {entt::type_id<Vector4>().hash(), InspectMutableVector4Field}
    };

    inline bool InspectImmutableStringField(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<std::string>();
        ImGui::Indent( 16.0f );

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", v.c_str());
        ImGui::Unindent( 16.0f );

        return true;
    }

    inline bool InspectImmutableTransformMatrix(entt::meta_data& field, entt::meta_any& componentData)
    {
        auto v = field.get(componentData).cast<TransformMatrix>();
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);
        ImGuiInputTextFlags textFlags = ImGuiInputTextFlags_ReadOnly;
        ImGui::Text("%s", "Read Only");
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (ImGui::BeginTable("Transform Values", 2, flags))
        {
            ImGui::TableSetupColumn(" Transform", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableSetupColumn("TransformComponent", ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::BeginDisabled();
            ImGui::DragFloat3("##position", &position.X, 0.1f);
            ImGui::EndDisabled();
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",field.prop("Position"_hs).value().cast<char const*>());

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::BeginDisabled();
            ImGui::DragFloat3("##Rotation", &position.X, 0.1f);
            ImGui::EndDisabled();
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",field.prop("Rotation"_hs).value().cast<char const*>());

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::BeginDisabled();
            ImGui::DragFloat3("##Scale", &position.X, 0.1f);
            ImGui::EndDisabled();
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",field.prop("Scale"_hs).value().cast<char const*>());


            /*
            ImGui::TableSetupColumn("##Labels", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##floats", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::AlignTextToFramePadding();
            ImGui::InputFloat3("##row", &position.X, "%f", textFlags);
            */
            ImGui::EndTable();
        }
        /*
        ImGui::InputFloat3(field.prop("Position"_hs).value().cast<char const*>(), &position.X, "%f", textFlags);
        ImGui::InputFloat3(field.prop("Rotation"_hs).value().cast<char const*>(), &rotation.X, "%f",textFlags);
        ImGui::InputFloat3(field.prop("Scale"_hs).value().cast<char const*>(), &scale.X, "%f",textFlags);
        */
        return true;
    }

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> _immutableDataInspectors
    {
        {entt::type_id<std::string>().hash(), InspectImmutableStringField},
        {entt::type_id<TransformMatrix>().hash(), InspectImmutableTransformMatrix},
        /*
         {entt::type_id<VmaMeshAsset>().hash(), inspect_mesh_asset},
         {entt::type_id<float>().hash(), inspect_float},
         {entt::type_id<Vector2>().hash(), inspect_vector2_field},
         {entt::type_id<Vector3>().hash(), inspect_vector3_field},
         {entt::type_id<Vector4>().hash(), inspect_vector4_field}*/
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
}
