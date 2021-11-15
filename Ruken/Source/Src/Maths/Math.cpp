
#include <complex>

#include "Maths/Math.hpp"
#include "Maths/Utility.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Pow(RkFloat const in_value, RkFloat const in_exponent) noexcept
{
    return std::powf(in_value, in_exponent);
}

RkFloat RUKEN_NAMESPACE::Exp(RkFloat const in_value) noexcept
{
    return std::expf(in_value);
}

RkFloat RUKEN_NAMESPACE::Exp2(RkFloat const in_value) noexcept
{
    return std::exp2f(in_value);
}

RkFloat RUKEN_NAMESPACE::Mod(RkFloat const in_value, RkFloat in_modulo) noexcept
{
    return std::modf(in_value, &in_modulo);
}

RkFloat RUKEN_NAMESPACE::Frac(RkFloat const in_value) noexcept
{
    return in_value - Floor(in_value);
}

RkFloat RUKEN_NAMESPACE::Sqrt(RkFloat const in_value) noexcept
{
    return std::sqrtf(in_value);
}

RkFloat RUKEN_NAMESPACE::InvSqrt(RkFloat const in_value) noexcept
{
    return 1.0F / Sqrt(in_value);
}