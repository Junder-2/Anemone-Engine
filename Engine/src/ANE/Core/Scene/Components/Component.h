#pragma once

namespace Engine
{
    // Maybe a horrible idea, remove if so.
    struct Component
    {
    private:
        const char* _name;

    protected:
        explicit Component(const char* name = "No Name Set") : _name(name)
        {
        }

    public:
        [[nodiscard("ToString() value not used")]] const char* ToString() const
        {
            return _name;
        }
    };
}

#define ANE_COMPONENT_INIT(type)     type() : Component(typeid(*this).name()) {} \
                                    type(const type&) = default;
