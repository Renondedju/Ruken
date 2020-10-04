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

#include "Meta/TupleIndex.hpp"
#include "Meta/TupleHasType.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename... TItems>
struct ItemHelper
{
    /**
     * \brief Checks if an item exists in the passed items of the class
     * \tparam TItem Item to look for
     */
    template <typename TItem>
    using ItemExists = TupleHasType<std::remove_const_t<TItem>, std::tuple<std::remove_const_t<TItems>...>>;

    /**
     * \brief Returns the index of an item from the passed items in the class
     * \tparam TField Item to look for
     */
    template <typename TField> requires ItemExists<TField>::value
    using ItemIndex = TupleIndex<std::remove_const_t<TField>, std::tuple<std::remove_const_t<TItems>...>>;
};


END_RUKEN_NAMESPACE