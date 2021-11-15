
#include <limits>

#include "Maths/MinMax.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::MinExceptZero(RkFloat const in_lhs, RkFloat const in_rhs) noexcept
{
    if (in_lhs == .0F)
        return in_rhs;

    if (in_rhs == .0F)
        return in_lhs;

    return Minimum(in_lhs, in_rhs);
}

RkFloat RUKEN_NAMESPACE::Maximum(RkFloat const in_lhs, RkFloat const in_rhs) noexcept
{
    return in_lhs < in_rhs ? in_rhs : in_lhs;
}

RkFloat RUKEN_NAMESPACE::Minimum(RkFloat const in_lhs, RkFloat const in_rhs) noexcept
{
    return in_lhs > in_rhs ? in_rhs : in_lhs;
}

RkFloat RUKEN_NAMESPACE::MinExceptZero(std::initializer_list<RkFloat> const in_list) noexcept
{
    RkFloat count = std::numeric_limits<RkFloat>::max();
    for (RkFloat const& element: in_list)
        count = MinExceptZero(count, element);

    return count;
}