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

#pragma region Constructors

constexpr Quaternion::Quaternion(RkFloat const in_w,
                                 RkFloat const in_x,
                                 RkFloat const in_y,
                                 RkFloat const in_z) noexcept:
    w {in_w},
    x {in_x},
    y {in_y},
    z {in_z}
{}

#pragma endregion

#pragma region Methods

constexpr Quaternion& Quaternion::Invert() noexcept
{
    x = -x;
    y = -y;
    z = -z;

    return *this;
}

constexpr RkFloat Quaternion::SqrLength() const noexcept
{
    return w * w + x * x + y * y + z * z;
}

constexpr Matrix3X3 Quaternion::GetRotationMatrix() const noexcept
{
    RkFloat const sqr_x (x * x);
    RkFloat const sqr_y (y * y);
    RkFloat const sqr_z (z * z);

    return Matrix3X3
    (
        1 - 2 * (sqr_y + sqr_z),
        2 * (x * y - z * w),
        2 * (x * z + y * w),

        2 * (x * y + z * w),
        1 - 2 * (sqr_x + sqr_z),
        2 * (y * z - x * w),

        2 * (x * z - y * w),
        2 * (y * z + x * w),
        1 - 2 * (sqr_x + sqr_y)
    );
}

#pragma endregion

#pragma region Operators

constexpr Quaternion Quaternion::operator*(Quaternion const& in_other) const noexcept
{
    return Quaternion(w * in_other.w - (x * in_other.x + y * in_other.y + z * in_other.z),
                      w * in_other.x + in_other.w * x + (y * in_other.z - in_other.y * z),
                      w * in_other.y + in_other.w * y + (z * in_other.x - in_other.z * x),
                      w * in_other.z + in_other.w * z + (x * in_other.y - in_other.x * y));
}

#pragma endregion