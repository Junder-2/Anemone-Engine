#pragma once
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"
#include "Types/Vector/FVector2.h"
#include "Types/Vector/IVector2.h"

#include "Types/Vector/FVector3.h"
#include "Types/Vector/IVector3.h"

namespace Engine
{
    typedef MathTypes::TVector<2, uint8> Vector2UInt8;
    typedef MathTypes::TVector<2, int8> Vector2Int8;
    typedef MathTypes::TVector<2, uint16> Vector2UInt16;
    typedef MathTypes::TVector<2, int16> Vector2Int16;
    typedef MathTypes::TVector<2, int> Vector2Int;
    typedef MathTypes::TVector<2, uint> Vector2UInt;

    typedef MathTypes::TVector<2, float> Vector2F;
    typedef MathTypes::TVector<2, double> Vector2D;

    typedef MathTypes::TVector<3, uint8> Vector3UInt8;
    typedef MathTypes::TVector<3, int8> Vector3Int8;
    typedef MathTypes::TVector<3, uint16> Vector3UInt16;
    typedef MathTypes::TVector<3, int16> Vector3Int16;
    typedef MathTypes::TVector<3, int> Vector3Int;
    typedef MathTypes::TVector<3, uint> Vector3UInt;

    typedef MathTypes::TVector<3, float> Vector3F;
    typedef MathTypes::TVector<3, double> Vector3D;
}
