#pragma once
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include "Types/Vector/FVector2.h"
#include "Types/Vector/IVector2.h"

namespace Engine
{
    typedef MathTypes::TVector<2, uint8> Vector2UInt8;
    typedef MathTypes::TVector<2, int8> Vector2Int8;
    typedef MathTypes::TVector<2, uint16> Vector2UInt16;
    typedef MathTypes::TVector<2, int16> Vector2Int16;
    typedef MathTypes::TVector<2, int> Vector2Int;
    typedef MathTypes::TVector<2, uint> Vector2UInt;

    typedef MathTypes::TVector<2, float> Vector2F;
}
