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

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements look at matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixLookAt
{};

// Requires a 4x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixLookAt<TRows, TColumns, std::enable_if_t<TRows == 4 && TColumns == 4>>
{
    /**
     * \brief Creates model matrix to look at a specific point in the world
     *
     * \param in_from Position to look from
     * \param in_to Position to look at
     * \param in_up Up vector
     *
     * \return Look at matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TRows> LookAtMatrix(
            Vector3m const& in_from,
            Vector3m const& in_to,
            Vector3m const& in_up) noexcept
    {
        Vector3m const forward   ((in_to - in_from   ).Normalized());
        Vector3m const right     (in_up.Cross(forward).Normalized());
        Vector3m const up        (forward.Cross(right));

        return {
            static_cast<RkFloat>(right.x), static_cast<RkFloat>(up.x), static_cast<RkFloat>(forward.x), static_cast<RkFloat>(in_from.x),
            static_cast<RkFloat>(right.y), static_cast<RkFloat>(up.y), static_cast<RkFloat>(forward.y), static_cast<RkFloat>(in_from.y),
            static_cast<RkFloat>(right.z), static_cast<RkFloat>(up.z), static_cast<RkFloat>(forward.z), static_cast<RkFloat>(in_from.z),
            0.0F, 0.0F, 0.0F, 1.0F
        };
    }
};

END_RUKEN_NAMESPACE