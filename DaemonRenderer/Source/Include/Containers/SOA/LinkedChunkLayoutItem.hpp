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
#include "Meta/ValueIndexer.hpp"

#include "Containers/SOA/LinkedChunkLayout.hpp"
#include "Containers/SOA/LinkedChunkLayoutView.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Describes a full linked chunk list layout as well as implementing setters/getters
 *        This class also comes in handy for Item inheritance
 * \tparam TTypes Types of the layout (SOA)
 */
template <typename... TTypes>
struct LinkedChunkLayoutItem : public std::tuple<TTypes...>
{
    // Making constructors available
    using std::tuple<TTypes...>::tuple;
    using std::tuple<TTypes...>::operator=;

    template <DAEsize... TItems>
    using MakeView = LinkedChunkLayoutView<std::index_sequence<TItems...>, SelectType<TItems, TTypes...>...>;
    using FullView = LinkedChunkLayoutView<std::make_index_sequence<sizeof...(TTypes)>, TTypes...>;

    using Layout   = LinkedChunkLayout<TTypes...>;
};

END_DAEMON_NAMESPACE