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

#include <type_traits>

BEGIN_DAEMON_NAMESPACE

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


/**
 * \brief  Allows the extraction of an index using a type in a parameter pack
 * \tparam TType  Type to look for
 * \tparam TTypes Parameter pack to look into
 */
template <typename TType, typename... TTypes>
inline constexpr std::size_t SelectIndex = typename decltype(InvertedSelect<TType>(
    Indexer<std::index_sequence_for<TTypes...>, TTypes...>{}
))::index;

// ----------------- Value indexed support (size_t) -----------------------

template <std::size_t TIndex, std::size_t TValue>
struct IndexedValue
{
    static constexpr std::size_t value = TValue;
    static constexpr std::size_t index = TIndex;
};

template <typename TIndices, std::size_t... TValues>
struct ValueIndexer;

template <std::size_t... TIndices, std::size_t... TValues>
struct ValueIndexer<std::index_sequence<TIndices...>, TValues...> : IndexedValue<TIndices, TValues>...
{ };

template <std::size_t TIndex, std::size_t TValue>
static IndexedValue<TIndex, TValue> ValueSelect(IndexedValue<TIndex, TValue>);

template <std::size_t TValue, std::size_t TIndex>
static IndexedValue<TIndex, TValue> ValueInvertedSelect(IndexedValue<TIndex, TValue>);

/**
 * \brief  Allows the extraction of a value using an index in an index sequence
 * \tparam TIndex Index of the value to extract
 * \tparam TValues Index sequence to look into
 *
 * \see http://loungecpp.wikidot.com/tips-and-tricks:indices
 */
template <std::size_t TIndex, std::size_t... TValues>
inline constexpr std::size_t SelectValue = typename decltype(ValueSelect<TIndex>(
    ValueIndexer<std::make_index_sequence<sizeof...(TValues)>, TValues...>{}
))::value;


/**
 * \brief  Allows the extraction of an index using a value in an index sequence
 * \tparam TValue  Value to look for
 * \tparam TValues Index sequence to look into
 */
template <std::size_t TValue, std::size_t... TValues>
inline constexpr std::size_t SelectValueIndex = typename decltype(ValueInvertedSelect<TValue>(
    ValueIndexer<std::make_index_sequence<sizeof...(TValues)>, TValues...>{}
))::index;

END_DAEMON_NAMESPACE