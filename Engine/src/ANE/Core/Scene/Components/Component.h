#pragma once
#include "MetaTypes.h"

namespace Engine
{
    struct Component
    {
    public:
        [[nodiscard("ToString() value not used")]] const char* ToString() const
        {
            return _name;
        }

    protected:
        Component(const char* name = "No Name Set") : _name(name) {}

    private:
        const char* _name;
    };
}

#define ANE_COMPONENT_INIT(type)     type() : Component(typeid(*this).name()) {} \
                                     type(const type&) = default;                \
                                     ~type() = default;
