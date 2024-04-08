#pragma once
#include <entt.hpp>

#include "Subsystem.h"

namespace Engine
{
    class SubsystemCollection
    {
    public:
        static std::unique_ptr<SubsystemCollection> Create()
        {
            return std::make_unique<SubsystemCollection>();
        }

        SubsystemCollection() = default;
        ~SubsystemCollection() = default;

        template<class T>
        T* GetSubSystem()
        {
            const size_t hashCode = typeid(T).hash_code();

            if(!_subsystems.contains(hashCode))
            {
                _subsystems.insert_or_assign(hashCode, std::make_unique<T>());
            }

            return dynamic_cast<T*>(_subsystems[hashCode].get());
        }
    private:
        entt::dense_map<size_t, std::unique_ptr<Subsystem>> _subsystems {};
    };
}
