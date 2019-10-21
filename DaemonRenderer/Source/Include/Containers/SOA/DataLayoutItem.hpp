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

#include <tuple>

#include "Config.hpp"

BEGIN_DAEMON_NAMESPACE

#pragma region Forward declarations

template <template <typename> class TContainer, typename... TLayoutTypes>
class DataLayout;

template <class TSequence, typename... TTypes>
struct DataLayoutView;

#pragma endregion 

/** 
 * \brienf DataItem class
 * This class is meant to describe a full data layout as well as implementing setters/getters
 * This class also comes in handy for Item inheritance
 */
template <template <typename> typename TContainer, typename... TTypes>
struct DataLayoutItem : public std::tuple<TTypes...>
{
    using Layout   = DataLayout<TContainer, TTypes...>;
    using FullView = DataLayoutView<std::make_index_sequence<sizeof...(TTypes)>, TTypes...>;

    // Making constructors available
    using std::tuple<TTypes...>::tuple;
    using std::tuple<TTypes...>::operator=;
};

END_DAEMON_NAMESPACE