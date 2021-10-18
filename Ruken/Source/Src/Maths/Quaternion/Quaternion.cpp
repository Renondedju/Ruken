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

#include "Maths/Math.hpp"
#include "Maths/Utility.hpp"
#include "Maths/Trigonometry.hpp"
#include "Maths/Quaternion/Quaternion.hpp"

USING_RUKEN_NAMESPACE

Quaternion::Quaternion(RkFloat const in_w,
                       RkFloat const in_x,
                       RkFloat const in_y,
                       RkFloat const in_z) noexcept:
    w {in_w},
    x {in_x},
    y {in_y},
    z {in_z}
{}

Quaternion::Quaternion(Degrees const in_angle_x,
                       Degrees const in_angle_y,
                       Degrees const in_angle_z) noexcept
{
    Radians const half_x_angle = in_angle_x / 2.0F;
    Radians const half_y_angle = in_angle_y / 2.0F;
    Radians const half_z_angle = in_angle_z / 2.0F;

    RkFloat const cos_x = Cos(half_x_angle);
    RkFloat const cos_y = Cos(half_y_angle);
    RkFloat const cos_z = Cos(half_z_angle);

    RkFloat const sin_x = Sin(half_x_angle);
    RkFloat const sin_y = Sin(half_y_angle);
    RkFloat const sin_z = Sin(half_z_angle);

    w = cos_x * cos_y * cos_z - sin_x * sin_y * sin_z;
    x = sin_x * cos_y * cos_z + cos_x * sin_y * sin_z;
    y = cos_x * sin_y * cos_z - sin_x * cos_y * sin_z;
    z = cos_x * cos_y * sin_z + sin_x * sin_y * cos_z;
}

template <RkBool TShortestPath>
Quaternion Quaternion::Lerp(Quaternion const& in_lhs,
                            Quaternion const& in_rhs,
                            RkFloat    const  in_ratio) noexcept
{
    RkFloat const conjugate_ratio = 1.0F - in_ratio;

    if constexpr (TShortestPath)
    {
        RkFloat const sign = Quaternion::Dot(in_lhs, in_rhs) < 0.0F ? -1.0F : 1.0F;

        return Quaternion(conjugate_ratio * in_lhs.w + sign * in_ratio * in_rhs.w,
                          conjugate_ratio * in_lhs.x + sign * in_ratio * in_rhs.x,
                          conjugate_ratio * in_lhs.y + sign * in_ratio * in_rhs.y,
                          conjugate_ratio * in_lhs.z + sign * in_ratio * in_rhs.z);
    }

    return Quaternion(conjugate_ratio * in_lhs.w + in_ratio * in_rhs.w,
                      conjugate_ratio * in_lhs.x + in_ratio * in_rhs.x,
                      conjugate_ratio * in_lhs.y + in_ratio * in_rhs.y,
                      conjugate_ratio * in_lhs.z + in_ratio * in_rhs.z);
}

template <RkBool TShortestPath>
Quaternion Quaternion::Slerp(Quaternion const& in_lhs,
                             Quaternion const& in_rhs,
                             RkFloat    const  in_ratio) noexcept
{
    RkFloat const dot_result {Quaternion::Dot(in_lhs, in_rhs)};
    Radians const abs_dot    {Abs   (dot_result)};
    Radians const theta      {ArcCos(abs_dot)};
    RkFloat const sin_theta  {Sin   (theta)};

    Quaternion const a = Quaternion::Scale(in_lhs, Sin(static_cast<Radians>(1.0F - in_ratio)) * static_cast<RkFloat>(theta) / sin_theta);

    if constexpr (TShortestPath)
    {
        RkFloat    const sign = dot_result < 0.0F ? -1.0F : 1.0F;
        Quaternion const b    = Quaternion::Scale(in_rhs, Sin(static_cast<Radians>(sign * in_ratio) * theta) / sin_theta);

        return Quaternion::Normalize(Quaternion::Add(a, b));
    }

    Quaternion const b = Quaternion::Scale(in_rhs, Sin(static_cast<Radians>(in_ratio) * theta) / sin_theta);

    return Quaternion::Normalize(Quaternion::Add(a, b));
}

RkFloat Quaternion::Dot(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept
{
    return in_lhs.w * in_rhs.w + in_lhs.x * in_rhs.x + in_lhs.y * in_rhs.y + in_lhs.z * in_rhs.z;
}

Quaternion Quaternion::Normalize(Quaternion const& in_quaternion) noexcept
{
    RkFloat const length = Quaternion::Length(in_quaternion);

    return Quaternion(
        in_quaternion.w / length,
        in_quaternion.x / length,
        in_quaternion.y / length,
        in_quaternion.z / length);
}

RkFloat Quaternion::SqrLength(Quaternion const& in_quaternion) noexcept
{
    return in_quaternion.w * in_quaternion.w +
           in_quaternion.x * in_quaternion.x +
           in_quaternion.y * in_quaternion.y +
           in_quaternion.z * in_quaternion.z;
}

RkFloat Quaternion::Length(Quaternion const& in_quaternion) noexcept
{
    return Sqrt(Quaternion::SqrLength(in_quaternion));
}

Quaternion Quaternion::Invert(Quaternion const& in_quaternion) noexcept
{
    return Quaternion(in_quaternion.w, -in_quaternion.x, -in_quaternion.y, -in_quaternion.z);
}

Quaternion Quaternion::Multiply(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept
{
    return Quaternion(in_lhs.w * in_rhs.w - (in_lhs.x * in_rhs.x + in_lhs.y * in_rhs.y + in_lhs.z * in_rhs.z),
                      in_lhs.w * in_rhs.x + in_rhs.w * in_lhs.x + (in_lhs.y * in_rhs.z - in_rhs.y * in_lhs.z),
                      in_lhs.w * in_rhs.y + in_rhs.w * in_lhs.y + (in_lhs.z * in_rhs.x - in_rhs.z * in_lhs.x),
                      in_lhs.w * in_rhs.z + in_rhs.w * in_lhs.z + (in_lhs.x * in_rhs.y - in_rhs.x * in_lhs.y));
}

Quaternion Quaternion::Scale(Quaternion const& in_quaternion, RkFloat const in_coefficient) noexcept
{
    return Quaternion(
        in_quaternion.w * in_coefficient,
        in_quaternion.x * in_coefficient,
        in_quaternion.y * in_coefficient,
        in_quaternion.z * in_coefficient);
}

Quaternion Quaternion::Add(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept
{
    return Quaternion(
        in_lhs.w + in_rhs.w,
        in_lhs.x + in_rhs.x,
        in_lhs.y + in_rhs.y,
        in_lhs.z + in_rhs.z);
}

Matrix3X3 Quaternion::RotationMatrix(Quaternion const& in_quaternion) noexcept
{
    RkFloat const sqr_x (in_quaternion.x * in_quaternion.x);
    RkFloat const sqr_y (in_quaternion.y * in_quaternion.y);
    RkFloat const sqr_z (in_quaternion.z * in_quaternion.z);

    return Matrix3X3
    (
        1 - 2 * (sqr_y + sqr_z),
        2 * (in_quaternion.x * in_quaternion.y - in_quaternion.z * in_quaternion.w),
        2 * (in_quaternion.x * in_quaternion.z + in_quaternion.y * in_quaternion.w),

        2 * (in_quaternion.x * in_quaternion.y + in_quaternion.z * in_quaternion.w),
        1 - 2 * (sqr_x + sqr_z),
        2 * (in_quaternion.y * in_quaternion.z - in_quaternion.x * in_quaternion.w),

        2 * (in_quaternion.x * in_quaternion.z - in_quaternion.y * in_quaternion.w),
        2 * (in_quaternion.y * in_quaternion.z + in_quaternion.x * in_quaternion.w),
        1 - 2 * (sqr_x + sqr_y)
    );
}
