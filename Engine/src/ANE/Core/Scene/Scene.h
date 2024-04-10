#pragma once
#include "ANE/Events/Event.h"
//#include "../Entity/Entity.h"
#include "entt.hpp"

namespace Engine
{
    class Entity;

    class Scene : std::enable_shared_from_this<Scene>
    {

    public:
        Scene();
        Scene(const Scene&) = default;

        ~Scene() = default;

        void OnEvent(Event& e);
        void OnUpdate(float timeStep);

        Entity Create(const char* name = "Untagged");
        entt::registry _registry;

    private:

        /*In order for ui layers to populate lists correctly they need access to the registry
         *If we move to a SceneManager service this probably won't be a problem and we can
         *remove this variable entirely
         **/
        //entt::registry _registry;

        friend class Entity;
    };
}
