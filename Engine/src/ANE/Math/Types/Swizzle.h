// _swizzle_func.hpp

#pragma once

namespace Engine::MathTypes
{
    #define GEN_SWIZZLE_VEC2_ENTRY(T, A, B)             \
        TVector<2, T> ## A ## B() const                 \
        {                                               \
            return TVector<2, T>(A, B);                 \
        }

    #define GEN_SWIZZLE_VEC3_ENTRY(T, A, B, C)          \
        TVector<3, T> ## A ## B ## C() const            \
        {                                               \
            return TVector<3, T>(A, B, C);              \
        }

    #define GEN_SWIZZLE_VEC4_ENTRY(T, A, B, C, D)       \
        TVector<4, T> ## A ## B ## C ## D() const       \
        {                                               \
            return TVector<4, T>(A, B, C, D);           \
        }

    #define GEN_SWIZZLE_VEC2_A01(T, A)                  \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, 1)                 \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, 0)

    #define GEN_SWIZZLE_VEC3_A01(T, A)                  \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, 1, 1)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, 1, 0)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, 0, 1)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, 0, 0)

    #define GEN_SWIZZLE_VEC3_AB01(T, A, B)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, B, 1)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, B, 0)

    #define GEN_SWIZZLE_VEC4_A01(T, A)                  \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, 1, 1, 1)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, 1, 1, 0)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, 1, 0, 1)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, 1, 0, 0)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, 0, 0, 0)

    #define GEN_SWIZZLE_VEC4_AB01(T, A, B)              \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, 1, 1)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, 1, 0)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, 0, 1)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, 0, 0)

    #define GEN_SWIZZLE_VEC4_ABC01(T, A, B, C)          \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, C, 1)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, C, 0)

    // Vec2

    #define GEN_SWIZZLE_VEC3_AXY(T, A, X, Y)            \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, X, X)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, X, Y)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Y, X)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Y, Y)

    #define GEN_SWIZZLE_VEC4_ABXY(T, A, B, X, Y)        \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, X, X)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, X, Y)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Y, X)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Y, Y)

    #define GEN_SWIZZLE_VEC2_FROM_VEC2(T, A, B)         \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, A)                 \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, B)                 \
        GEN_SWIZZLE_VEC2_ENTRY(T, B, A)                 \
        GEN_SWIZZLE_VEC2_ENTRY(T, B, B)

    #define GEN_SWIZZLE_VEC3_FROM_VEC2(T, A, B)         \
        GEN_SWIZZLE_VEC3_AXY(T, A, A, B)                \
        GEN_SWIZZLE_VEC3_AXY(T, B, A, B)                \

    #define GEN_SWIZZLE_VEC4_FROM_VEC2(T, A, B)         \
        GEN_SWIZZLE_VEC4_ABXY(T, A, A, A, B)            \
        GEN_SWIZZLE_VEC4_ABXY(T, A, B, A, B)            \
        GEN_SWIZZLE_VEC4_ABXY(T, B, A, A, B)            \
        GEN_SWIZZLE_VEC4_ABXY(T, B, B, A, B)

    #define GEN_SWIZZLE_VEC2_01(T, A, B)                \
        GEN_SWIZZLE_VEC2_A01(T, A)                      \
        GEN_SWIZZLE_VEC2_A01(T, B)                      \
        GEN_SWIZZLE_VEC3_A01(T, A)                      \
        GEN_SWIZZLE_VEC3_A01(T, B)                      \
        GEN_SWIZZLE_VEC3_AB01(T, A, B)                  \
        GEN_SWIZZLE_VEC4_A01(T, A)                      \
        GEN_SWIZZLE_VEC4_A01(T, B)                      \
        GEN_SWIZZLE_VEC4_AB01(T, A, B)

    #define GEN_SWIZZLE_VEC2(T, A, B)                   \
        GEN_SWIZZLE_VEC2_FROM_VEC2(T, A, B)             \
        GEN_SWIZZLE_VEC3_FROM_VEC2(T, A, B)             \
        GEN_SWIZZLE_VEC4_FROM_VEC2(T, A, B)

    // Vec3

    #define GEN_SWIZZLE_VEC2_AXYZ(T, A, X, Y, Z)        \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, X)                 \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, Y)                 \
        GEN_SWIZZLE_VEC2_ENTRY(T, A, Z)                 \

    #define GEN_SWIZZLE_VEC2_FROM_VEC3(T, A, B, C)      \
        GEN_SWIZZLE_VEC2_AXYZ(T, A, A, B, C)            \
        GEN_SWIZZLE_VEC2_AXYZ(T, B, A, B, C)            \
        GEN_SWIZZLE_VEC2_AXYZ(T, C, A, B, C)            \

    #define GEN_SWIZZLE_VEC3_AXYZ(T, A, X, Y, Z)        \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, X, X)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, X, Y)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, X, Z)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Y, X)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Y, Y)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Y, Z)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Z, X)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Z, Y)              \
        GEN_SWIZZLE_VEC3_ENTRY(T, A, Z, Z)              \

    #define GEN_SWIZZLE_VEC3_FROM_VEC3(T, A, B, C)      \
        GEN_SWIZZLE_VEC3_AXYZ(T, A, A, B, C)            \
        GEN_SWIZZLE_VEC3_AXYZ(T, B, A, B, C)            \
        GEN_SWIZZLE_VEC3_AXYZ(T, C, A, B, C)

    #define GEN_SWIZZLE_VEC4_ABXYZ(T, A, B, X, Y, Z)    \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, X, X)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, X, Y)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, X, Z)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Y, X)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Y, Y)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Y, Z)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Z, X)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Z, Y)           \
        GEN_SWIZZLE_VEC4_ENTRY(T, A, B, Z, Z)

    #define GEN_SWIZZLE_VEC4_FROM_VEC3(T, A, B, C)      \
        GEN_SWIZZLE_VEC4_ABXYZ(T, A, A, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, A, B, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, A, C, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, B, A, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, B, B, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, B, C, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, C, A, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, C, B, A, B, C)        \
        GEN_SWIZZLE_VEC4_ABXYZ(T, C, C, A, B, C)

    #define GEN_SWIZZLE_VEC3_01(T, A, B, C)             \
        GEN_SWIZZLE_VEC2_A01(T, A)                      \
        GEN_SWIZZLE_VEC2_A01(T, B)                      \
        GEN_SWIZZLE_VEC2_A01(T, C)                      \
        GEN_SWIZZLE_VEC3_A01(T, A)                      \
        GEN_SWIZZLE_VEC3_A01(T, B)                      \
        GEN_SWIZZLE_VEC3_A01(T, C)                      \
        GEN_SWIZZLE_VEC3_AB01(T, A, B)                  \
        GEN_SWIZZLE_VEC4_AB01(T, A, B)                  \
        GEN_SWIZZLE_VEC4_A01(T, A)                      \
        GEN_SWIZZLE_VEC4_A01(T, B)                      \
        GEN_SWIZZLE_VEC4_A01(T, C)                      \
        GEN_SWIZZLE_VEC4_ABC01(T, A, B, C)

    #define GEN_SWIZZLE_VEC3(T, A, B, C)                \
        GEN_SWIZZLE_VEC2_FROM_VEC3(T, A, B, C)          \
        GEN_SWIZZLE_VEC3_FROM_VEC3(T, A, B, C)          \
        GEN_SWIZZLE_VEC4_FROM_VEC3(T, A, B, C)

}

