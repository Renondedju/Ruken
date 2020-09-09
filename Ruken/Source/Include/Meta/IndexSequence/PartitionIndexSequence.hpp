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
#include "Meta/IndexSequence/ConcatenateIndexSequence.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Partitions a given sequence into 2 sub sequences via a given predicate
 * \tparam TPredicate Predicate to use for the partitioning
 *
 * Ex:
 * template <std::size_t v>
 * struct is_odd : std::integral_constant<bool, v & 1> {};
 *
 * \tparam TSequence Sequence to partition
 */
template <template <std::size_t TValue> class TPredicate, typename TSequence>
struct PartitionIndexSequence;

template <template <std::size_t TValue> class TPredicate>
struct PartitionIndexSequence<TPredicate, std::index_sequence<>>
{
    using FirstSequence  = std::index_sequence<>;
    using SecondSequence = std::index_sequence<>;
};

template <template <std::size_t TValue> class TPredicate, std::size_t TId, std::size_t... TIds>
struct PartitionIndexSequence<TPredicate, std::index_sequence<TId, TIds...>>
{
    private:

        using Included = std::index_sequence<TId>;
        using Excluded = std::index_sequence<>;

        using TailPartition = PartitionIndexSequence<TPredicate, std::index_sequence<TIds...> >;

        static const bool outcome = TPredicate<TId>::value;

    public:

        using FirstSequence  = ConcatenateIndexSequenceT<std::conditional_t<outcome, Included, Excluded>, typename TailPartition::FirstSequence>;
        using SecondSequence = ConcatenateIndexSequenceT<std::conditional_t<outcome, Excluded, Included>, typename TailPartition::SecondSequence>;
};

END_RUKEN_NAMESPACE