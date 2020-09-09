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

#pragma once

#include <typeindex>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Concatenates multiple std::index_sequence together
 * \tparam TSequences Sequences to concatenate
 */
template <typename... TSequences>
struct ConcatenateIndexSequence;

/**
 * \brief Concatenates multiple std::index_sequence together (type shorthand)
 * \tparam TSequences Sequences to concatenate
 */
template <typename... TSequences>
using ConcatenateIndexSequenceT = typename ConcatenateIndexSequence<TSequences...>::Type;

template <std::size_t... TIds>
struct ConcatenateIndexSequence<std::index_sequence<TIds...>>
{
    using Type = std::index_sequence<TIds...>;
};

template <std::size_t... TIds1, std::size_t... TIds2, typename... TTailSequences>
struct ConcatenateIndexSequence<std::index_sequence<TIds1...>,
              std::index_sequence<TIds2...>,
              TTailSequences...>
{
    using Type = ConcatenateIndexSequenceT<std::index_sequence<TIds1..., TIds2...>, TTailSequences...>;
};

END_RUKEN_NAMESPACE