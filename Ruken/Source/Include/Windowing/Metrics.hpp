
#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct Position2D
{
    RkInt32 x;
    RkInt32 y;
};

struct Extent2D
{
    RkInt32 width;
    RkInt32 height;
};

struct Scale2D
{
    RkFloat x;
    RkFloat y;
};

struct Rect2D
{
    Position2D position;
    Extent2D   extent;
};

END_RUKEN_NAMESPACE