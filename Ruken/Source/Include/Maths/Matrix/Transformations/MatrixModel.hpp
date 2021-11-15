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
#include "Maths/Quaternion/Quaternion.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements look at matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixModel
{};

// Requires a 4x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixModel<TRows, TColumns, std::enable_if_t<TRows == 4 && TColumns == 4>>
{
    /**
     * \brief Model matrix, also called TRS
     *
     * \param in_position Model position
     * \param in_rotation Model rotation
     * \param in_scale Model scale
     *
     * \return Model (TRS) matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> ModelMatrix(
            Vector3m   const& in_position,
            Quaternion const& in_rotation,
            Vector3m   const& in_scale) noexcept
    {
        RkFloat const sqr_x (in_rotation.x * in_rotation.x);
        RkFloat const sqr_y (in_rotation.y * in_rotation.y);
        RkFloat const sqr_z (in_rotation.z * in_rotation.z);

        return {
            static_cast<RkFloat>((1 - 2 * (sqr_y + sqr_z)) * in_scale.x),
            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.y - in_rotation.z * in_rotation.w) * in_scale.y),
            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.z + in_rotation.y * in_rotation.w) * in_scale.z),
            static_cast<RkFloat>(in_position.x),

            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.y + in_rotation.z * in_rotation.w) * in_scale.x),
            static_cast<RkFloat>((1 - 2 * (sqr_x + sqr_z)) * in_scale.y),
            static_cast<RkFloat>(2 * (in_rotation.y * in_rotation.z - in_rotation.x * in_rotation.w) * in_scale.z),
            static_cast<RkFloat>(in_position.y),

            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.z - in_rotation.y * in_rotation.w) * in_scale.x),
            static_cast<RkFloat>(2 * (in_rotation.y * in_rotation.z + in_rotation.x * in_rotation.w) * in_scale.y),
            static_cast<RkFloat>((1 - 2 * (sqr_x + sqr_y)) * in_scale.z),
            static_cast<RkFloat>(in_position.z),

            0.0F,
            0.0F,
            0.0F,
            1.0F
        };
    }
};

END_RUKEN_NAMESPACE