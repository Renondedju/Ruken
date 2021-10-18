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

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning(push)
#pragma warning(disable: 4201) // warning C4201: nonstandard extension used : nameless struct/union

/**
 * \brief Generic vector layout
 * \tparam TDimensions Number of dimensions of the vector
 * \tparam TUnderlyingType Underlying type of the vector
 */
template <RkSize TDimensions, typename TUnderlyingType>
struct VectorLayout
{
    TUnderlyingType data[TDimensions] {};
};

/**
 * \brief Generic two dimensional vector
 * \tparam TUnderlyingType Underlying type of the vector
 */
template <typename TUnderlyingType>
struct VectorLayout<2, TUnderlyingType>
{
    union
    {
        TUnderlyingType data[2] {};
        struct
        {
            TUnderlyingType x;
            TUnderlyingType y;
        };
    };
};

/**
 * \brief Generic three dimensional vector
 * \tparam TUnderlyingType Underlying type of the vector
 */
template <typename TUnderlyingType>
struct VectorLayout<3, TUnderlyingType>
{
    union
    {
        TUnderlyingType data[3] {};
        struct
        {
            TUnderlyingType x;
            TUnderlyingType y;
            TUnderlyingType z;
        };
    };
};

#pragma warning(pop)

END_RUKEN_NAMESPACE