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

#include "Maths/Trigonometry.hpp"
#include "Maths/Matrix/Matrix4X4.hpp"
#include "Maths/Quaternion/Quaternion.hpp"

USING_RUKEN_NAMESPACE

constexpr Matrix4X4 Matrix4X4::ClipSpace() noexcept
{
    return Matrix4X4(1.0F,  0.0F,  0.0F,  0.0F,
                     0.0F, -1.0F,  0.0F,  0.0F,
                     0.0F,  0.0F,  0.5F,  0.5F,
                     0.0F,  0.0F,  0.0F,  1.0F); 
}

constexpr Matrix4X4 Matrix4X4::TranslationMatrix(Vector3f const& in_translation) noexcept
{
    return Matrix4X4(1.0F,  0.0F,  0.0F,  in_translation.x,
                     0.0F,  1.0F,  0.0F,  in_translation.y,
                     0.0F,  0.0F,  1.0F,  in_translation.z,
                     0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix4X4 Matrix4X4::RotationMatrixX(Radians const in_angle) noexcept
{
    RkFloat const cos = Cos(in_angle);
    RkFloat const sin = Sin(in_angle);

    return Matrix4X4(1.0F,  0.0F,  0.0F,  0.0F,
                     0.0F,  cos,   -sin,  0.0F,
                     0.0F,  sin,    cos,  0.0F,
                     0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix4X4 Matrix4X4::RotationMatrixY(Radians const in_angle) noexcept
{
    RkFloat const cos = Cos(in_angle);
    RkFloat const sin = Sin(in_angle);

    return Matrix4X4( cos,  0.0F,   sin,  0.0F,
                     0.0F,  1.0F,  0.0F,  0.0F,
                    - sin,  0.0F,   cos,  0.0F,
                     0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix4X4 Matrix4X4::RotationMatrixZ(Radians const in_angle) noexcept
{
    RkFloat const cos = Cos(in_angle);
    RkFloat const sin = Sin(in_angle);

    return Matrix4X4( cos,  -sin,  0.0F,  0.0F,
                      sin,   cos,  0.0F,  0.0F,
                     0.0F,  0.0F,  1.0F,  0.0F,
                     0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix4X4 Matrix4X4::RotationMatrix(
    Radians const in_angle_x,
    Radians const in_angle_y,
    Radians const in_angle_z) noexcept
{
    return RotationMatrixZ(in_angle_x).GetMultiplied(RotationMatrixY(in_angle_y)).GetMultiplied(RotationMatrixX(in_angle_z));
}

constexpr Matrix4X4 Matrix4X4::ScaleMatrix(Vector3f const& in_scale) noexcept
{
    return Matrix4X4(in_scale.x,  0.0F      ,  0.0F      ,  0.0F,
                     0.0F      ,  in_scale.y,  0.0F      ,  0.0F,
                     0.0F      ,  0.0F      ,  in_scale.z,  0.0F,
                     0.0F      ,  0.0F      ,  0.0F      ,  1.0F); 
}

Matrix4X4 Matrix4X4::PerspectiveProjectionMatrix(Degrees const in_fov,
                                                 RkFloat const in_aspect,
                                                 RkFloat const in_near,
                                                 RkFloat const in_far) noexcept
{
    RkFloat const scale = 1.0F / Tan(in_fov / 2.0F);

    return Matrix4X4(scale / in_aspect,  0.0F , 0.0F                                 ,  0.0F,
                     0.0F             , -scale, 0.0F                                 ,  0.0F,
                     0.0F             ,  0.0F , in_far / (in_near - in_far)          , -1.0F,
                     0.0F             ,  0.0F , in_near * in_far / (in_near - in_far),  0.0F);
}

constexpr Matrix4X4 Matrix4X4::OrthogonalProjectionMatrix(
        RkFloat const in_left, RkFloat const in_right, RkFloat const in_bottom,
        RkFloat const in_top,  RkFloat const in_near , RkFloat const in_far) noexcept
{
      return Matrix4X4(2.0F / (in_right - in_left),
      0.0F,
      0.0F,
      0.0F,

      0.0F,
      2.0F / (in_bottom - in_top),
      0.0F,
      0.0F,

      0.0F,
      0.0F,
      1.0F / (in_near - in_far),
      0.0F,

      -(in_right + in_left) / (in_right - in_left),
      -(in_bottom + in_top) / (in_bottom - in_top),
      in_near / (in_near - in_far),
      1.0F);
}

constexpr Matrix4X4 Matrix4X4::LookAtMatrix(
        Vector3f const& in_from,
        Vector3f const& in_to,
        Vector3f const& in_up) noexcept
{
    Vector3f const forward   ((in_to - in_from   ).GetNormalized());
    Vector3f const right     (in_up.Cross(forward).GetNormalized());
    Vector3f const up        (forward.Cross(right));

    return Matrix4X4
    (
        right.x, up.x, forward.x, in_from.x,
        right.y, up.y, forward.y, in_from.y,
        right.z, up.z, forward.z, in_from.z,
        0.0F,    0.0F, 0.0F,      1.0F
    );
}

constexpr Matrix4X4 Matrix4X4::ModelMatrix(
        Vector3f   const& in_position,
        Quaternion const& in_rotation,
        Vector3f   const& in_scale) noexcept
{
    RkFloat const sqr_x (in_rotation.x * in_rotation.x);
    RkFloat const sqr_y (in_rotation.y * in_rotation.y);
    RkFloat const sqr_z (in_rotation.z * in_rotation.z);

    return Matrix4X4
    (
        (1 - 2 * (sqr_y + sqr_z)) * in_scale.x,
        2 * (in_rotation.x * in_rotation.y - in_rotation.z * in_rotation.w) * in_scale.y,
        2 * (in_rotation.x * in_rotation.z + in_rotation.y * in_rotation.w) * in_scale.z,
        in_position.x,

        2 * (in_rotation.x * in_rotation.y + in_rotation.z * in_rotation.w) * in_scale.x,
        (1 - 2 * (sqr_x + sqr_z)) * in_scale.y,
        2 * (in_rotation.y * in_rotation.z - in_rotation.x * in_rotation.w) * in_scale.z,
        in_position.y,

        2 * (in_rotation.x * in_rotation.z - in_rotation.y * in_rotation.w) * in_scale.x,
        2 * (in_rotation.y * in_rotation.z + in_rotation.x * in_rotation.w) * in_scale.y,
        (1 - 2 * (sqr_x + sqr_y)) * in_scale.z,
        in_position.z,

        0.0F,
        0.0F,
        0.0F,
        1.0F
    );
}