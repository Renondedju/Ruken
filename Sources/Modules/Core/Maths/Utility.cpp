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

RkFloat RUKEN_NAMESPACE::Round(RkFloat const in_value) noexcept
{
    return std::roundf(in_value);
}

RkFloat RUKEN_NAMESPACE::Floor(RkFloat const in_value) noexcept
{
    return std::floorf(in_value);
}

//RkFloat RUKEN_NAMESPACE::CopySign(RkFloat const in_value, RkFloat const in_sign) noexcept
//{
//    return std::copysignf(in_value, in_sign);
//}
