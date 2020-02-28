/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#pragma once

#include "Config.hpp"
#include "Containers/Tuple.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Finds the index of the first occurence of TType in TTuple
 * \note If the type isn't found, this will throw a hard compilation error
 * \tparam TType Type occurence to look for
 * \tparam TTuple Tuple to look into
 */
template <class TType, class TTuple>
struct TupleIndex;

template <class TType, class... TTypes>
struct TupleIndex<TType, Tuple<TType, TTypes...>>
{
    static constexpr DAEsize value = 0;
};

template <class TType, class TFirst, class... TTypes>
struct TupleIndex<TType, Tuple<TFirst, TTypes...>>
{
    static constexpr DAEsize value = 1 + TupleIndex<TType, Tuple<TTypes...>>::value;
};

/**
 * \brief Finds the index of the first occurence of TType in TTuple
 * \note If the type isn't found, this will throw a hard compilation error
 * \tparam TType Type occurence to look for
 * \tparam TTuple Tuple to look into
 */
template <class TType, class TTuple>
inline constexpr DAEsize TupleIndexV = TupleIndex<TType, TTuple>::value;

END_DAEMON_NAMESPACE