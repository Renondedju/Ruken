/**
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

#include "Maths/Vector/New/Vector.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Vector 3 data layout
 * \tparam TDataType Underlying data type
 */
template <typename TDataType>
struct Vector3Layout
{
    using UnderlyingType = TDataType;
    static constexpr RkSize dimensions = 3ULL;

    #pragma warning(push)
    #pragma warning(disable : 4201) // Warning C4201 nonstandard extension used: nameless struct/union

    union
    {
        TDataType data[3] {0, 0, 0};

        struct
        {
            TDataType x;
            TDataType y;
            TDataType z;
        };
    };

    #pragma warning(pop)

    /**
     * \brief Generic constructor
     * \tparam TValuesType Type of passed values
     * \param in_values Values to init the vector with
     */
    template <typename... TValuesType> requires (sizeof...(TValuesType) == dimensions)
    constexpr Vector3Layout(TValuesType... in_values) noexcept:
        data {static_cast<UnderlyingType>(in_values)...}
    {}

    constexpr Vector3Layout() noexcept:
        data {0, 0, 0}
    {}
};

/**
 * \brief Vector 3 class
 * \tparam TDataType Underlying data type
 */
template <typename TDataType>
using Vector3 = Vector<Vector3Layout<TDataType>>;

using Vector3i = Vector3<RkInt>;
using Vector3f = Vector3<RkFloat>;

END_RUKEN_NAMESPACE