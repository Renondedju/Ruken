
#include <cmath>

#include "Maths/Trigonometry.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Cos(Radians const in_angle) noexcept
{
    return std::cosf(static_cast<RkFloat>(in_angle));
}

Radians RUKEN_NAMESPACE::ArcCos(RkFloat const in_dot) noexcept
{
    return static_cast<Radians>(std::acosf(in_dot));
}

RkFloat RUKEN_NAMESPACE::Sin(Radians const in_angle) noexcept
{
    return std::sinf(static_cast<RkFloat>(in_angle));
}

Radians RUKEN_NAMESPACE::ArcSin(RkFloat const in_angle) noexcept
{
    return static_cast<Radians>(std::asinf(in_angle));
}

RkFloat RUKEN_NAMESPACE::Tan(Radians const in_angle) noexcept
{
    return std::tanf(static_cast<RkFloat>(in_angle));
}

Radians RUKEN_NAMESPACE::ArcTan(RkFloat const in_angle) noexcept
{
    return static_cast<Radians>(std::atanf(in_angle));
}