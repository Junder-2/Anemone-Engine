#pragma once

#define BIT(x) (1 << (x))
#define TWO_BIT_MASK(x) ((0b11 << ((x)*2)))

// todo: Should move to more appropriate place
struct IntPair //struct that can be used in an unordered map
{
    IntPair(const int x, const int y) : X(x), Y(y) {}
    int X;
    int Y;

    bool operator<(const IntPair& rhs) const { return X != rhs.X ? X < rhs.X : Y < rhs.Y; } // for ordered map
    bool operator==(const IntPair& rhs) const { return X == rhs.X && Y == rhs.Y;} // for hash
};

template<>
struct std::hash<IntPair>
{
    std::size_t operator() (const IntPair& v) const { return std::hash<int>()(v.X) ^ hash<int>()(v.Y) << 1; }
};
