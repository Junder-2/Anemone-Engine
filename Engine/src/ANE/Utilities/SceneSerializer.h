#pragma once
#include "ANE/Core/Scene/Scene.h"
#include "toml.hpp"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Layers/Layer.h"

namespace Engine
{
    class EditorLayer;

    class SceneSerializer
    {
    public:
        SceneSerializer();
        toml::array EntitySerializer(Entity& entity);
        void Serialize(const std::shared_ptr<Scene>& scene); // toml
        void SerializeBinary(); // binary
        std::shared_ptr<Scene> Deserialize(const char* key, EditorLayer* layer) const; // toml
        std::shared_ptr<Scene> DeserializeBinary(); // binary

        void WriteToFile(std::string filepath, const toml::table& content);
    private:
        //map?
        std::string _filepath; // path + key
        std::string _binaryFilepath;
    };
}
