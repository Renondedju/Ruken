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

#include "Config.hpp"

#include "Meta/IndexSequence/PartitionIndexSequence.hpp"
#include "Meta/IndexSequence/ConcatenateIndexSequence.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Quicksort an std::index_sequence
 * \tparam TCompare Comparer class
 * \tparam TSequence Sequence to quicksort
 */
template <template <std::size_t TLhs, std::size_t TRhs> class TCompare, typename TSequence>
struct QuicksortIndexSequence;

/**
 * \brief Quicksort an std::index_sequence (type shorthand)
 * \tparam TCompare Comparer class
 * \tparam TSequence Sequence to quicksort
 */
template <template <std::size_t TLhs, std::size_t TRhs> class TCompare, typename TSequence>
using QuicksortIndexSequenceT = typename QuicksortIndexSequence<TCompare, TSequence>::Type;

template <template <std::size_t TLhs, std::size_t TRhs> class TCompare>
struct QuicksortIndexSequence<TCompare, std::index_sequence<>>
{
    using Type = std::index_sequence<>;
};

template <template <std::size_t TLhs, std::size_t TRhs> class TCompare, std::size_t TId, std::size_t... TIds>
struct QuicksortIndexSequence<TCompare, std::index_sequence<TId, TIds...>>
{
    template <std::size_t TValue>
    using Predicate = TCompare<TValue, TId>;

    using Partitions = PartitionIndexSequence<Predicate, std::index_sequence<TIds...>>;

    using FirstSequence  = typename Partitions::FirstSequence;
    using SecondSequence = typename Partitions::SecondSequence;

    using Type = ConcatenateIndexSequenceT<
        QuicksortIndexSequenceT<TCompare, FirstSequence>,
        std::index_sequence<TId>,
        QuicksortIndexSequenceT<TCompare, SecondSequence>>;
};

END_DAEMON_NAMESPACE