#include "anepch.h"
#include "SceneSerializer.h"
#include "ANE/Core/Scene/Components/TagComponent.h"


namespace Engine
{
    SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene) : _scene(scene)
    {

    }

    std::string SceneSerializer::EntitySerializer(Entity entity)
    {
         return "";
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {

        //toml::array arr;
        // _scene->_registry.view<TagComponent>().each([&](auto entityID)
        // {
        //     Entity entity = {entityID, _scene.get()};
        //
        //     if (!entity) return;
        //
        //     arr.push_back(EntitySerializer(entity));
        // });

        // const toml::table table{
        //     {"Scene", "Name"},
        //     {"Entities:", arr}
        // };

        // std::ofstream output(filepath);
        // output << table << "\n\n";

        //ANE_ELOG(table);
    }

    void SceneSerializer::SerializeBinary(const std::string& filepath)
    {
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        return false;
    }

    bool SceneSerializer::DeserializeBinary(const std::string& filepath)
    {
        // not implemented
        ANE_ASSERT(false);
        return false;
    }
}
