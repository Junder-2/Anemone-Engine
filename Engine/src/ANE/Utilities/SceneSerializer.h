#pragma once
#include "ANE/Core/Scene/Scene.h"
#include "toml.hpp"
#include "ANE/Core/Entity/Entity.h"

namespace Engine
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const std::shared_ptr<Scene>& scene);
        std::string EntitySerializer(Entity entity);
        void Serialize(const std::string& filepath); // toml
        void SerializeBinary(const std::string& filepath); // binary
        bool Deserialize(const std::string& filepath); // toml
        bool DeserializeBinary(const std::string& filepath); // binary
    private:
        std::shared_ptr<Scene> _scene;
    };
}
