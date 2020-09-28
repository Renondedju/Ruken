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

#include "ECS/Safety/FieldType.hpp"

BEGIN_RUKEN_NAMESPACE

template <FieldType... TFields>
struct FieldHelper
{
    // Checks if all the passed fields are constant or not
    using Readonly = std::conjunction<std::is_const<TFields>...>;

    /**
     * \brief Checks if a field exists in the passed fields of the class, const modifiers are ignored
     * \tparam TField Field to check for
     */
    template <FieldType TField>
    using FieldExists = TupleHasType<std::remove_const_t<TField>, std::tuple<std::remove_const_t<TFields>...>>;

    /**
     * \brief Finds the index of a field, const modifiers are ignored
     * \tparam TField Field to find. This field must exist in the passed fields
     */
    template <FieldType TField> requires FieldExists<TField>::value
    using FieldIndex = TupleIndex<std::remove_const_t<TField>, std::tuple<std::remove_const_t<TFields>...>>;
};

END_RUKEN_NAMESPACE