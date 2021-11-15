
#include "ECS/Range.hpp"

USING_RUKEN_NAMESPACE

Range::Range(RkSize const in_begin, RkSize const in_size):
    begin {in_begin},
    size  {in_size}
{ }

RkSize Range::ReduceRight() noexcept
{
    // Cannot reduce an empty range
    if (size == 0ULL)
        return 0ULL;

    ++begin;
    return --size;
}

RkSize Range::ExpandLeft() noexcept
{
    // Cannot expand if we are already on the maximum range possible on the left
    if (begin == 0ULL)
        return size;

    --begin;
    return ++size;
}

RkSize Range::ExpandRight() noexcept
{
    return ++size;
}

RkBool Range::Contains(RkSize const in_position) const noexcept
{
    return in_position >= begin && in_position < begin + size;
}
