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

#include <tuple>

#include "Build/Namespace.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class is used to fetch only certain fields when using LinkedChunkLayout::Get
 * \tparam TSequence index sequence of the elements in the original LinkedChunkLayout
 * \tparam TTypes Types of the referred elements
 *
 * \see LinkedChunkLayout::Get
 */
template <class TSequence, typename... TTypes>
struct LinkedChunkLayoutView : public std::tuple<TTypes&...>
{
    using Sequence = TSequence;

    // Making constructors available
    using std::tuple<TTypes&...>::tuple;
    using std::tuple<TTypes&...>::operator=;
};

template <template <std::size_t...> class TSequence, typename... TTypes, std::size_t... TIndices>
struct LinkedChunkLayoutView<TSequence<TIndices...>, TTypes...> : public std::tuple<TTypes&...>
{
    using Sequence = TSequence<TIndices...>;

    // Making constructors available
    using std::tuple<TTypes&...>::tuple;
    using std::tuple<TTypes&...>::operator=;
};

END_DAEMON_NAMESPACE