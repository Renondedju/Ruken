
#include <cmath>

#include "Maths/Utility.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Abs(RkFloat const in_value) noexcept
{
    return in_value < 0.0F ? -in_value : in_value;
}

RkFloat RUKEN_NAMESPACE::Ceil(RkFloat const in_value) noexcept
{
    return std::ceilf(in_value);
}

RkFloat RUKEN_NAMESPACE::Clamp(RkFloat const in_value, RkFloat const in_min, RkFloat const in_max) noexcept
{
    return in_value < in_min ? in_min : in_value > in_max ? in_max : in_value;
}

RkFloat RUKEN_NAMESPACE::Clamp01(RkFloat const in_value) noexcept
{
    return in_value < 0.0F ? 0.0F : in_value > 1.0F ? 1.0F : in_value;
}

RkFloat RUKEN_NAMESPACE::Round(RkFloat const in_value) noexcept
{
    return std::roundf(in_value);
}

RkFloat RUKEN_NAMESPACE::Floor(RkFloat const in_value) noexcept
{
    return std::floorf(in_value);
}

RkFloat RUKEN_NAMESPACE::CopySign(RkFloat const in_value, RkFloat const in_sign) noexcept
{
    return std::copysignf(in_value, in_sign);
}
