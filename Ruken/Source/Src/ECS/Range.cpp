/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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
