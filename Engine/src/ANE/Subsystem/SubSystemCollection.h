#pragma once
#include <entt.hpp>
#include "SubSystem.h"

namespace Engine
{
    class SubSystemCollection
    {
    public:
        static std::unique_ptr<SubSystemCollection> Create()
        {
            return std::make_unique<SubSystemCollection>();
        }

        SubSystemCollection() = default;
        ~SubSystemCollection() = default;

        template<class T>
        T* GetSubSystem()
        {
            const size_t hashCode = typeid(T).hash_code();
            const char* name = typeid(T).name();

            if(!_subsystems.contains(hashCode))
            {
                _subsystems.insert_or_assign(hashCode, std::make_unique<T>());

                ANE_ELOG_INFO("Instancing new subsystem of {}", name);
            }

            return dynamic_cast<T*>(_subsystems[hashCode].get());
        }
    private:
        entt::dense_map<size_t, std::unique_ptr<SubSystem>> _subsystems {};
    };
}
