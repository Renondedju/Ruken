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

#include <type_traits>

#include "Config.hpp"

BEGIN_RUKEN_NAMESPACE

template <std::size_t TIndex, typename TType>
struct Indexed
{
    using Type                         = TType;
    static constexpr std::size_t index = TIndex;
};

template <typename TIndices, typename... TTypes>
struct Indexer;

template <std::size_t... TIndices, typename... TTypes>
struct Indexer<std::index_sequence<TIndices...>, TTypes...> : Indexed<TIndices, TTypes>...
{ };

template <std::size_t TIndex, typename TType>
static Indexed<TIndex, TType> Select(Indexed<TIndex, TType>);

template <typename TType, std::size_t TIndex>
static Indexed<TIndex, TType> InvertedSelect(Indexed<TIndex, TType>);

/**
 * \brief  Allows the extraction of a type using an index in a parameter pack
 * \tparam TIndex Index of the type to extract
 * \tparam TTypes Parameter pack to look into
 *
 * \see http://loungecpp.wikidot.com/tips-and-tricks:indices
 */
template <std::size_t TIndex, typename... TTypes>
using SelectType = typename decltype(Select<TIndex>(
    Indexer<std::index_sequence_for<TTypes...>, TTypes...>{}
))::Type;

END_RUKEN_NAMESPACE