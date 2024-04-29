#pragma once

#define EDITABLEHASH "is_editable"_hs
#define EDITABLE prop(EDITABLEHASH, true)
#define NOT_EDITABLE prop(EDITABLEHASH, false)
namespace Engine
{
    struct Component
    {
    private:
        const char* _name;

    protected:

        Component(const char* name = "No Name Set") : _name(name) {}


    public:
        [[nodiscard("ToString() value not used")]] const char* ToString() const
        {
            return _name;
        }

    };
}

#define ANE_COMPONENT_INIT(type)     type() : Component(typeid(*this).name()) {} \
                                     type(const type&) = default;                \
                                     ~type() = default;
