#pragma once
#include "entt.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "PhysicsUtilities.h"
#include "ANE/Core/Scene/Components/Components.h"
#include "ANE/Math/Types/TransformMatrix.h"
#include "ANE/Math/Types/Vector2.h"

#include "ANE/Math/Types/Vector3.h"
#include "ANE/Math/Types/Vector4.h"

namespace Engine
{
    using namespace entt::literals;


    inline bool inspect_mutable_string_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<std::string>();
        bool propertyWritten = false;
        bool writable = field.prop(EDITABLEHASH).value().cast<bool>();
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());
        if (ImGui::InputText(field.prop("display_name"_hs).value().cast<const char*>(), buffer, sizeof(buffer)))
        {
            propertyWritten = field.set(component_data, std::string(buffer));
        }
        return propertyWritten;
    }

    inline bool inspect_mutable_transform_matrix(entt::meta_data& field, entt::meta_any& component_data)
    {
        TransformMatrix newMatrix;
        auto v = field.get(component_data).cast<TransformMatrix>();
        bool propertyWritten = false;
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);
        ImGui::Text("%s", field.prop("display_name"_hs).value().cast<const char*>());
        if (ImGui::DragFloat3(field.prop("Position"_hs).value().cast<char const*>(), &position.X, 0.1f))
        {
            v.SetPosition(position);
            propertyWritten = field.set(component_data, v);
        }
        if (ImGui::DragFloat3(field.prop("Rotation"_hs).value().cast<char const*>(), &rotation.X, 0.1f))
        {
            v.SetRotation(rotation, true);
            propertyWritten = field.set(component_data, v);
        }
        if (ImGui::DragFloat3(field.prop("Scale"_hs).value().cast<char const*>(), &scale.X, 0.1f))
        {
            v.SetScale(scale);
            propertyWritten = field.set(component_data, v);
        }
        return propertyWritten;
    }

    inline bool inspect_mutable_colliders(entt::meta_data& field, entt::meta_any& component_data)
    {
        bool changed = false;
        auto v = field.get(component_data).cast<std::vector<Collider*>>();
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
                    changed = true;
                }
                if (ImGui::DragFloat3("Local Rotation", &rotation.X, 0.1f))
                {
                    collider->SetRotation(rotation, true);
                    changed = true;
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
                            changed = true;
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
                            changed = true;
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
                            changed = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::DragFloat("Height", &height, 0.1f))
                        {
                            capsuleCollider->SetHeight(radius);
                            changed = true;
                        }
                    }
                    break;
                }
                ImGui::TreePop();
            }
        }
        if (changed) field.set(component_data, v);
        return changed;
    }

    inline bool InspectRigidBody(entt::meta_data& field, entt::meta_any& componentData)
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

    inline bool inspect_colliders(entt::meta_data& field, entt::meta_any& component_data)
    {
        bool changed = false;
        auto v = field.get(component_data).cast<std::vector<Collider*>>();
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
                    changed = true;
                }
                if (ImGui::DragFloat3("Local Rotation", &rotation.X, 0.1f))
                {
                    collider->SetRotation(rotation, true);
                    changed = true;
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
                            changed = true;
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
                            changed = true;
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
                            changed = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::DragFloat("Height", &height, 0.1f))
                        {
                            capsuleCollider->SetHeight(radius);
                            changed = true;
                        }
                    }
                    break;
                }
                ImGui::TreePop();
            }
        }
        if (changed) field.set(component_data, v);
        return changed;
    }

    inline bool inspect_mutable_float(entt::meta_data& field, entt::meta_any& component_data)
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

    inline bool inspect_mutable_mesh_asset(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<VmaMeshAsset>();
        bool propertyWritten = false;

        ImGui::Text("%s", v.Name.c_str());
        return propertyWritten;
    }

    inline bool inspect_mutable_vector2_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<Vector2>();
        bool propertyWritten = false;

        if (ImGui::DragFloat2(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = field.set(component_data, v);
        }
        return propertyWritten;
    }


    inline bool inspect_mutable_vector3_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<Vector3>();
        bool propertyWritten = false;

        if (ImGui::DragFloat3(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = field.set(component_data, v);
        }
        return propertyWritten;
    }

    inline bool inspect_mutable_vector4_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<Vector4>();
        bool propertyWritten = false;

        if (ImGui::DragFloat4(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten = field.set(component_data, v);
        }
        return propertyWritten;
    }

    using FieldInspectorFn = bool (*)(entt::meta_data& field, entt::meta_any& component_data);

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> g_mutable_data_inspectors
    {
        {entt::type_id<std::string>().hash(), inspect_mutable_string_field},
        {entt::type_id<TransformMatrix>().hash(), inspect_mutable_transform_matrix},
        {entt::type_id<std::vector<Collider*>>().hash(), inspect_mutable_colliders},
        {entt::type_id<VmaMeshAsset>().hash(), inspect_mutable_mesh_asset},
        {entt::type_id<float>().hash(), inspect_mutable_float},
        {entt::type_id<Vector2>().hash(), inspect_mutable_vector2_field},
        {entt::type_id<Vector3>().hash(), inspect_mutable_vector3_field},
        {entt::type_id<Vector4>().hash(), inspect_mutable_vector4_field}

    };


    inline bool inspect_immutable_string_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<std::string>();
        ImGui::Text("%s", v.c_str());
        return true;
    }

    inline bool inspect_immutable_transform_matrix(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<TransformMatrix>();
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);
        ImGuiInputTextFlags textFlags = ImGuiInputTextFlags_ReadOnly;
        ImGui::Text("%s", "Read Only");
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (ImGui::BeginTable("Transform Values", 2, flags))
        {
            ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 2; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
                }
            }
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

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> g_immutable_data_inspectors
    {
        {entt::type_id<std::string>().hash(), inspect_immutable_string_field},
        {entt::type_id<TransformMatrix>().hash(), inspect_immutable_transform_matrix},
        /*
         {entt::type_id<std::vector<Collider*>>().hash(), inspect_colliders},
         {entt::type_id<VmaMeshAsset>().hash(), inspect_mesh_asset},
         {entt::type_id<float>().hash(), inspect_float},
         {entt::type_id<Vector2>().hash(), inspect_vector2_field},
         {entt::type_id<Vector3>().hash(), inspect_vector3_field},
         {entt::type_id<Vector4>().hash(), inspect_vector4_field}*/
    };

    template <typename ...Args>
    inline auto InvokeMetaFunction(entt::meta_type meta, entt::id_type func_id, Args&&...args)
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
    inline auto InvokeMetaFunction(entt::id_type id, entt::id_type func_id, Args&&...args)
    {
        return InvokeMetaFunction(entt::resolve(id), func_id, std::forward<Args>(args)...);
    }
}
