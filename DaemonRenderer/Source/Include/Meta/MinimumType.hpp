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

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Returns the minimum type required to store an object containing ``TSize`` bytes
 * \tparam TSize Number of required bytes
 * \tparam TExcessType Excess type, default is DAEvoid
 */
template <DAEuint8 TSize, typename TExcessType = DAEvoid>
struct MinimumType {
    using Type = std::conditional_t<TSize == 0u , DAEvoid,
                 std::conditional_t<TSize <= 8u , DAEuint8,
                 std::conditional_t<TSize <= 16u, DAEuint16,
                 std::conditional_t<TSize <= 32u, DAEuint32,
                 std::conditional_t<TSize <= 64u, DAEuint64,
        TExcessType>>>>>;
};

/**
 * \brief Helper type alias for MinimumType<TSize>::Type
 * \tparam TSize Number of required bytes
 * \tparam TExcessType Excess type, default is DAEvoid
 */
template <DAEuint8 TSize, typename TExcessType = DAEvoid>
using MinimumTypeT = typename MinimumType<TSize, TExcessType>::Type;

END_DAEMON_NAMESPACE
