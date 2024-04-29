#pragma once
#include "entt.hpp"
#include "imgui.h"
#include "ANE/Core/Scene/Components/Components.h"
#include "ANE/Math/Types/TransformMatrix.h"
#include "ANE/Math/Types/Vector2.h"

#include "ANE/Math/Types/Vector3.h"
#include "ANE/Math/Types/Vector4.h"


namespace Engine
{
    using namespace entt::literals;

    inline bool inspect_string_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<std::string>();
        bool propertyWritten = false;

        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), v.c_str());
        if(ImGui::InputText(field.prop("display_name"_hs).value().cast<const char*>(),buffer,sizeof(buffer)))
        {
            propertyWritten = field.set(component_data,std::string(buffer));

        }
        return propertyWritten;
    }
    inline bool inspect_transform_matrix(entt::meta_data& field, entt::meta_any& component_data)
    {
        TransformMatrix newMatrix;
        auto v = field.get(component_data).cast<TransformMatrix>();
        bool propertyWritten = false;
        Vector3 position = v.GetPosition();
        Vector3 scale = v.GetScale();
        Vector3 rotation = v.GetEulerAngles(true);
        ImGui::Text("%s",field.prop("display_name"_hs).value().cast<const char*>());
        if (ImGui::DragFloat3(field.prop("Position"_hs).value().cast<char const*>(), &position.X, 0.1f))
        {
            v.SetPosition(position);
            propertyWritten = field.set(component_data,v);
        }
        if (ImGui::DragFloat3(field.prop("Rotation"_hs).value().cast<char const*>(), &rotation.X, 0.1f))
        {
            v.SetRotation(rotation,true);
            propertyWritten = field.set(component_data,v);

        }
        if (ImGui::DragFloat3(field.prop("Scale"_hs).value().cast<char const*>(), &scale.X, 0.1f))
        {
            v.SetScale(scale);
            propertyWritten = field.set(component_data,v);

        }
        return propertyWritten;
    }
    inline bool inspect_collider(entt::meta_data& field, entt::meta_any& component_data)
    {
        //auto v = field.get(component_data).cast<rp3d::Collider>();
        bool propertyWritten = false;
        ImGui::Text("%s","Collider");
        return propertyWritten;

    }
    inline bool inspect_collider_shape_type(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<CollisionShapeType>();
        bool propertyWritten = false;

        ImGui::Text("%f",(CollisionShapeType)v);
        return propertyWritten;
        //ImGui::Text("%d",v);
    }
    inline bool inspect_float(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<float>();
        bool propertyWritten = false;

        if(ImGui::DragFloat(field.prop("display_name"_hs).value().cast<char const*>(),&v,0.1f))
        {
             propertyWritten = field.set(component_data,v); //insufficient for dirty flag!!!
        }
        return propertyWritten;
        //ImGui::Text("%d",v);
    }
    inline bool inspect_mesh_asset(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<VmaMeshAsset>();
        bool propertyWritten = false;

        ImGui::Text("%s", v.Name.c_str());
        return propertyWritten;
    }

    inline bool inspect_vector2_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<Vector2>();
        bool propertyWritten = false;

        if (ImGui::DragFloat2(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten =  field.set(component_data, v);
        }
        return propertyWritten;
    }

    inline bool inspect_vector3_field(entt::meta_data& field, entt::meta_any& component_data)
    {
        auto v = field.get(component_data).cast<Vector3>();
        bool propertyWritten = false;

        if (ImGui::DragFloat3(field.prop("display_name"_hs).value().cast<char const*>(), &v.X, 0.1f))
        {
            propertyWritten =  field.set(component_data, v);
        }
        return propertyWritten;
    }
    inline bool inspect_vector4_field(entt::meta_data& field, entt::meta_any& component_data)
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

    inline static std::unordered_map<entt::id_type, FieldInspectorFn> g_data_inspectors
    {
        {entt::type_id<std::string>().hash(), inspect_string_field},
        {entt::type_id<TransformMatrix>().hash(), inspect_transform_matrix},
          {entt::type_id<rp3d::Collider>().hash(), inspect_collider},
        {entt::type_id<CollisionShapeType>().hash(), inspect_collider_shape_type},
        {entt::type_id<VmaMeshAsset>().hash(), inspect_mesh_asset},
        {entt::type_id<float>().hash(), inspect_float},
        {entt::type_id<Vector2>().hash(), inspect_vector2_field},
        {entt::type_id<Vector3>().hash(), inspect_vector3_field},
        {entt::type_id<Vector4>().hash(), inspect_vector4_field}
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
