#pragma once
#include "LoggingUtilities.h"
#include "entt.hpp"
#include "imgui.h"
#include "ANE/Core/Scene/Components/Components.h"
#include "ANE/Math/Types/TransformMatrix.h"

#include "ANE/Math/Types/Vector3.h"


namespace Engine
{
    using namespace entt::literals;

    inline void inspect_string_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<std::string>();
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());
        ImGui::InputText(field.prop("display_name"_hs).value().cast<const char*>(),buffer,sizeof(buffer));
        field.set(component_data,std::string(buffer));
    }
    inline void inspect_transform_component(entt::meta_data& field, entt::meta_any& component_data)
    {

        TransformMatrix newMatrix;
        auto v = field.get(component_data).cast<TransformMatrix>();
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles();
        ImGui::Text("%s",field.prop("display_name"_hs).value().cast<const char*>());
        if (ImGui::DragFloat3(field.prop("Position"_hs).value().cast<char const*>(), &position.X, 0.1f))
        {
            v.SetPosition(position);
        }
        if (ImGui::DragFloat3(field.prop("Rotation"_hs).value().cast<char const*>(), &rotation.X, 0.1f))
        {
            v.SetRotation(rotation,true);
        }
        if (ImGui::DragFloat3(field.prop("Scale"_hs).value().cast<char const*>(), &scale.X, 0.1f))
        {
            // v.SetScale(scale);
        }
        field.set(component_data,v);
    }
    inline void inspect_collider_shape_type(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<CollisionShapeType>();
        ImGui::Text("%d",v);

    }

    inline void inspect_vector3_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<std::string>();
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());
        ImGui::InputText(field.prop("display_name"_hs).value().cast<const char*>(),buffer,sizeof(buffer));
        field.set(component_data,std::string(buffer));
    }
    inline void inspect_vector2_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<std::string>();
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());
        ImGui::InputText(field.prop("display_name"_hs).value().cast<const char*>(),buffer,sizeof(buffer));
        field.set(component_data,std::string(buffer));
    }
    using FieldInspectorFn = void (*)(entt::meta_data& field, entt::meta_any& component_data);

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> g_data_inspectors
    {
        {entt::type_id<std::string>().hash(), inspect_string_field},
        {entt::type_id<TransformMatrix>().hash(), inspect_transform_component},
        //{entt::type_id<ColliderComponent>().hash(), inspect_collider_shape_type}
    };




    template<typename ...Args>
    inline auto InvokeMetaFunction(entt::meta_type meta, entt::id_type func_id, Args&&...args)
    {
        if(!meta)
        {
            ANE_LOG_ERROR("No relevant meta type registered");

        }
        if(auto meta_function = meta.func(func_id);meta_function)
        {
            return meta_function.invoke({},std::forward<Args>(args)...);
        }
        return entt::meta_any{};
    }

    template<typename ...Args>
    inline auto InvokeMetaFunction(entt::id_type id, entt::id_type func_id, Args&&...args)
    {
        return InvokeMetaFunction(entt::resolve(id),func_id,std::forward<Args>(args)...);
    }

}
