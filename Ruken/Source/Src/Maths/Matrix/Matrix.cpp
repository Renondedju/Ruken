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
#include "Maths/Matrix/Matrix.hpp"
#include "Maths/Quaternion/Quaternion.hpp"

USING_RUKEN_NAMESPACE

Matrix<4, 4> Matrix<4, 4>::ClipSpace() noexcept
{
    return Matrix<4, 4>(1.0F,  0.0F,  0.0F,  0.0F,
                        0.0F, -1.0F,  0.0F,  0.0F,
                        0.0F,  0.0F,  0.5F,  0.5F,
                        0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix<4, 4> Matrix<4, 4>::TranslationMatrix(Vector3m const& in_translation) noexcept
{
    return Matrix<4, 4>(1.0F,  0.0F,  0.0F,  static_cast<RkFloat>(in_translation.x),
                        0.0F,  1.0F,  0.0F,  static_cast<RkFloat>(in_translation.y),
                        0.0F,  0.0F,  1.0F,  static_cast<RkFloat>(in_translation.z),
                        0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix<4, 4> Matrix<4, 4>::RotationMatrixX(Radians const in_angle) noexcept
{
    RkFloat const cos = Cos(in_angle);
    RkFloat const sin = Sin(in_angle);

    return Matrix<4, 4>(1.0F,  0.0F,  0.0F,  0.0F,
                        0.0F,  cos,   -sin,  0.0F,
                        0.0F,  sin,    cos,  0.0F,
                        0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix<4, 4> Matrix<4, 4>::RotationMatrixY(Radians const in_angle) noexcept
{
    RkFloat const cos = Cos(in_angle);
    RkFloat const sin = Sin(in_angle);

    return Matrix<4, 4>( cos,  0.0F,   sin,  0.0F,
                        0.0F,  1.0F,  0.0F,  0.0F,
                       - sin,  0.0F,   cos,  0.0F,
                        0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix<4, 4> Matrix<4, 4>::RotationMatrixZ(Radians const in_angle) noexcept
{
    RkFloat const cos = Cos(in_angle);
    RkFloat const sin = Sin(in_angle);

    return Matrix<4, 4>( cos,  -sin,  0.0F,  0.0F,
                         sin,   cos,  0.0F,  0.0F,
                        0.0F,  0.0F,  1.0F,  0.0F,
                        0.0F,  0.0F,  0.0F,  1.0F); 
}

Matrix<4, 4> Matrix<4, 4>::RotationMatrix(
    Radians const in_angle_x,
    Radians const in_angle_y,
    Radians const in_angle_z) noexcept
{
    return RotationMatrixZ(in_angle_x) * RotationMatrixY(in_angle_y) * RotationMatrixX(in_angle_z);
}

Matrix<4, 4> Matrix<4, 4>::ScaleMatrix(Vector3m const& in_scale) noexcept
{
    return Matrix<4, 4>(static_cast<RkFloat>(in_scale.x),  0.0F      ,  0.0F      ,  0.0F,
                        0.0F      ,  static_cast<RkFloat>(in_scale.y),  0.0F      ,  0.0F,
                        0.0F      ,  0.0F      ,  static_cast<RkFloat>(in_scale.z),  0.0F,
                        0.0F      ,  0.0F      ,  0.0F      ,  1.0F); 
}

Matrix<4, 4> Matrix<4, 4>::PerspectiveProjectionMatrix(
    Degrees const in_fov,
    RkFloat const in_aspect,
    Meters  const in_near,
    Meters  const in_far) noexcept
{
    RkFloat const scale = 1.0F / Tan(in_fov / 2.0F);

    return Matrix<4, 4>(scale / in_aspect,  0.0F , 0.0F                                 ,  0.0F,
                        0.0F             , -scale, 0.0F                                 ,  0.0F,
                        0.0F             ,  0.0F , static_cast<RkFloat>(in_far / (in_near - in_far))          , -1.0F,
                        0.0F             ,  0.0F , static_cast<RkFloat>(in_near * in_far / (in_near - in_far)),  0.0F);
}

Matrix<4, 4> Matrix<4, 4>::OrthogonalProjectionMatrix(
        Meters const in_left, Meters const in_right, Meters const in_bottom,
        Meters const in_top,  Meters const in_near , Meters const in_far) noexcept
{
      return Matrix<4, 4>(
      static_cast<RkFloat>(2.0F / (in_right - in_left)),
      0.0F,
      0.0F,
      0.0F,

      0.0F,
      static_cast<RkFloat>(2.0F / (in_bottom - in_top)),
      0.0F,
      0.0F,

      0.0F,
      0.0F,
      static_cast<RkFloat>(1.0F / (in_near - in_far)),
      0.0F,

      static_cast<RkFloat>(-(in_right + in_left) / (in_right - in_left)),
      static_cast<RkFloat>(-(in_bottom + in_top) / (in_bottom - in_top)),
      static_cast<RkFloat>(in_near / (in_near - in_far)),
      1.0F);
}

Matrix<4, 4> Matrix<4, 4>::LookAtMatrix(
        Vector3m const& in_from,
        Vector3m const& in_to,
        Vector3m const& in_up) noexcept
{
    Vector3m const forward   ((in_to - in_from   ).Normalized());
    Vector3m const right     (in_up.Cross(forward).Normalized());
    Vector3m const up        (forward.Cross(right));

    return Matrix<4, 4>
    (
        static_cast<RkFloat>(right.x), static_cast<RkFloat>(up.x), static_cast<RkFloat>(forward.x), static_cast<RkFloat>(in_from.x),
        static_cast<RkFloat>(right.y), static_cast<RkFloat>(up.y), static_cast<RkFloat>(forward.y), static_cast<RkFloat>(in_from.y),
        static_cast<RkFloat>(right.z), static_cast<RkFloat>(up.z), static_cast<RkFloat>(forward.z), static_cast<RkFloat>(in_from.z),
        0.0F,    0.0F, 0.0F,      1.0F
    );
}

Matrix<4, 4> Matrix<4, 4>::ModelMatrix(
        Vector3m   const& in_position,
        Quaternion const& in_rotation,
        Vector3m   const& in_scale) noexcept
{
    RkFloat const sqr_x (in_rotation.x * in_rotation.x);
    RkFloat const sqr_y (in_rotation.y * in_rotation.y);
    RkFloat const sqr_z (in_rotation.z * in_rotation.z);

    return Matrix<4, 4>
    (
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
    );
}