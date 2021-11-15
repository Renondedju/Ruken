
#include <cmath>

#include "Maths/Trigonometry.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Cos(Radians const in_angle) noexcept
{
    return std::cosf(static_cast<RkFloat>(in_angle));
}

RkFloat RUKEN_NAMESPACE::ArcCos(Radians const in_angle) noexcept
{
    return std::acosf(static_cast<RkFloat>(in_angle));
}

RkFloat RUKEN_NAMESPACE::Sin(Radians const in_angle) noexcept
{
    return std::sinf(static_cast<RkFloat>(in_angle));
}

RkFloat RUKEN_NAMESPACE::ArcSin(Radians in_angle) noexcept
{
    return std::asinf(static_cast<RkFloat>(in_angle));
}

RkFloat RUKEN_NAMESPACE::Tan(Radians const in_angle) noexcept
{
    return std::tanf(static_cast<RkFloat>(in_angle));
}

RkFloat RUKEN_NAMESPACE::ArcTan(Radians in_angle) noexcept
{
    return std::atanf(static_cast<RkFloat>(in_angle));
}