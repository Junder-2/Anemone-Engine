#pragma once
#include "Component.h"

namespace Engine
{
    struct AttachmentsComponent : Component
    {
    public:
        ANE_COMPONENT_INIT(AttachmentsComponent)

        std::vector<Component*> components;

        void SortComponentList(){

        }
        std::vector<Component*> GetComponentList(){
            return components;
        }
        /*
        void AddToComponentList(const char* componentName){
            std::string name = std::string(componentName);
            components.emplace_back(name);
        }
        */

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>> AddComponent(T component){
            //components.push_back(component);
        }
        void AddComponent(Component* component){
            components.push_back(component);
        }
    protected:

    };

}
