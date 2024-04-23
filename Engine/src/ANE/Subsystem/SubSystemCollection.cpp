#include "anepch.h"
#include "SubSystemCollection.h"

#include <ranges>

namespace Engine
{
    SubSystemCollection::~SubSystemCollection()
    {
        for (const auto& val : _subsystems | std::views::values)
        {
            val->Free();
        }
        _subsystems.clear();
    }
}
