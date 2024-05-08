#pragma once

#include <type_traits> //for std::underlying_type

#define BIT(x) (1 << (x))
#define TWO_BIT_MASK(x) ((0b11 << ((x)*2)))

template<typename E>
constexpr std::underlying_type_t<E> EnumCast(E e)
{
    return static_cast<std::underlying_type_t<E>>(e);
}

// Define bitwise operators for an enum class, allowing usage as bitmasks.
#define ENUM_CLASS_OPERATORS(Enum)                                  \
    inline constexpr Enum operator|(Enum lhs, Enum rhs) {           \
        return static_cast<Enum>(                                   \
            static_cast<std::underlying_type_t<Enum>>(lhs) |        \
            static_cast<std::underlying_type_t<Enum>>(rhs));        \
    }                                                               \
    inline constexpr Enum operator&(Enum lhs, Enum rhs) {           \
        return static_cast<Enum>(                                   \
            static_cast<std::underlying_type_t<Enum>>(lhs) &        \
            static_cast<std::underlying_type_t<Enum>>(rhs));        \
    }                                                               \
    inline constexpr Enum operator^(Enum lhs, Enum rhs) {           \
        return static_cast<Enum>(                                   \
            static_cast<std::underlying_type_t<Enum>>(lhs) ^        \
            static_cast<std::underlying_type_t<Enum>>(rhs));        \
    }                                                               \
    inline constexpr Enum operator~(Enum E) {                       \
        return static_cast<Enum>(                                   \
            ~static_cast<std::underlying_type_t<Enum>>(E));         \
    }                                                               \
    inline Enum& operator|=(Enum& lhs, Enum rhs) {                  \
        return lhs = static_cast<Enum>(                             \
            static_cast<std::underlying_type_t<Enum>>(lhs) |        \
            static_cast<std::underlying_type_t<Enum>>(lhs));        \
    }                                                               \
    inline Enum& operator&=(Enum& lhs, Enum rhs) {                  \
        return lhs = static_cast<Enum>(                             \
            static_cast<std::underlying_type_t<Enum>>(lhs) &        \
            static_cast<std::underlying_type_t<Enum>>(lhs));        \
    }                                                               \
    inline Enum& operator^=(Enum& lhs, Enum rhs) {                  \
        return lhs = static_cast<Enum>(                             \
            static_cast<std::underlying_type_t<Enum>>(lhs) ^        \
            static_cast<std::underlying_type_t<Enum>>(lhs));        \
    }
