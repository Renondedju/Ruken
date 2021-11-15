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

#include "Maths/Trigonometry.hpp"
#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Matrix/Operations/MatrixAccess.hpp"

#include "Types/Units/Angle/Angle.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements the rotation matrix on the y axis
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixRotationY
{};

// Y rotation matrix requires at least a 3x3 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixRotationY<TRows, TColumns, std::enable_if_t<TRows >= 3 && TColumns >= 3>>
{
    /**
     * \brief Creates a rotation matrix for the Y axis
     * \param in_angle Angle of the Y axis rotation (in radians)
     * \return Y axis rotation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> RotationMatrixY(Radians const in_angle) noexcept
    {
        RkFloat const cos = Cos(in_angle);
        RkFloat const sin = Sin(in_angle);

        return Matrix<3, 3> {
             cos,  0.0F,   sin,
            0.0F,  1.0F,  0.0F,
            -sin,  0.0F,   cos
        };
    }
};

END_RUKEN_NAMESPACE