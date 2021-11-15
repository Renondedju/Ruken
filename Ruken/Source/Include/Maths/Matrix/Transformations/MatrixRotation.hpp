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

#include "Types/Units/Angle/Angle.hpp"
#include "Maths/Matrix/MatrixForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements the 3D rotation matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixRotation
{};

// 3D rotation matrix requires at least a 3x3 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixRotation<TRows, TColumns, std::enable_if_t<TRows >= 3 && TColumns >= 3>>
{
    /**
     * \brief Creates a rotation matrix
     *
     * \param in_angle_x X Angle of the matrix
     * \param in_angle_y Y Angle of the matrix
     * \param in_angle_z Z Angle of the matrix
     *
     * \return Rotation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> RotationMatrix(Radians const in_angle_x, Radians const in_angle_y, Radians const in_angle_z) noexcept
    {
        RkFloat const cos_x = Cos(in_angle_x);
        RkFloat const sin_x = Sin(in_angle_x);
        
        RkFloat const cos_y = Cos(in_angle_y);
        RkFloat const sin_y = Sin(in_angle_y);
        
        RkFloat const cos_z = Cos(in_angle_z);
        RkFloat const sin_z = Sin(in_angle_z);

        // The resulting matrix will implicitly be converted to the requested size if needed
        return Matrix<3, 3> {
             cos_z * cos_y - sin_z * sin_x * sin_y,
            -sin_z * cos_x,
             cos_z * sin_y + sin_z * sin_x * cos_y,

             sin_z * cos_y + cos_z * sin_x * sin_y,
             cos_z * cos_x,
             sin_z * sin_y - cos_z * sin_x * cos_y,

            -cos_x * sin_y,
             sin_x,
             cos_x * cos_y
        };
    }

    /**
     * \brief Axis angle rotation matrix
     * \param in_axis Axis to rotate around (must be normalized)
     * \param in_angle Angle of the rotation
     * \return Rotation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> RotationMatrix(Vector3m const& in_axis, Radians const in_angle)
    {
        RkFloat const cos_angle = Cos(in_angle);
        RkFloat const sin_angle = Sin(in_angle);

        // The matrix will implicitly be converted to the requested size if needed
        return Matrix<3, 3> {
            cos_angle + static_cast<RkFloat>(in_axis.x * in_axis.x) * (1 - cos_angle),
            in_axis.x * in_axis.y * (1 - cos_angle) - in_axis.z * sin_angle,
            in_axis.x * in_axis.z * (1 - cos_angle) + in_axis.y * sin_angle,
            in_axis.y * in_axis.x * (1 - cos_angle) + in_axis.z * sin_angle,
            cos_angle + static_cast<RkFloat>(in_axis.y * in_axis.y) * (1 - cos_angle),
            in_axis.y * in_axis.z * (1 - cos_angle) - in_axis.x * sin_angle,
            in_axis.z * in_axis.x * (1 - cos_angle) - in_axis.y * sin_angle,
            in_axis.z * in_axis.y * (1 - cos_angle) + in_axis.x * sin_angle,
            cos_angle + static_cast<RkFloat>(in_axis.z * in_axis.z) * (1 - cos_angle),
        };
    }

    /**
     * \brief Creates a rotation matrix
     * \return Rotation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> RotationMatrix(Quaternion const in_value) noexcept
    {
        RkFloat const sqr_x (in_value.x * in_value.x);
        RkFloat const sqr_y (in_value.y * in_value.y);
        RkFloat const sqr_z (in_value.z * in_value.z);

        // The matrix will implicitly be converted to the requested size if needed
        return Matrix<3, 3> {
            1 - 2 * (sqr_y + sqr_z),
            2 * (in_value.x * in_value.y - in_value.z * in_value.w),
            2 * (in_value.x * in_value.z + in_value.y * in_value.w),

            2 * (in_value.x * in_value.y + in_value.z * in_value.w),
            1 - 2 * (sqr_x + sqr_z),
            2 * (in_value.y * in_value.z - in_value.x * in_value.w),

            2 * (in_value.x * in_value.z - in_value.y * in_value.w),
            2 * (in_value.y * in_value.z + in_value.x * in_value.w),
            1 - 2 * (sqr_x + sqr_y)
        };
    }
};

END_RUKEN_NAMESPACE