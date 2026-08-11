#pragma once

#include "Core/Maths/Utility.hpp"

USING_RUKEN_NAMESPACE

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

template <RkBool TShortestPath>
Quaternion Quaternion::Lerp(Quaternion const& in_lhs,
                            Quaternion const& in_rhs,
                            RkFloat    const  in_ratio) noexcept
{
    RkFloat const conjugate_ratio = 1.0F - in_ratio;

    if constexpr (TShortestPath)
    {
        RkFloat const sign = Dot(in_lhs, in_rhs) < 0.0F ? -1.0F : 1.0F;

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
    RkFloat const dot_result {Dot(in_lhs, in_rhs)};
    RkFloat const abs_dot    {Abs   (dot_result)};
    Radians const theta      {ArcCos(abs_dot)};
    RkFloat const sin_theta  {Sin   (theta)};

    Quaternion const a = Scale(in_lhs, Sin(static_cast<Radians>(1.0F - in_ratio)) * static_cast<RkFloat>(theta) / sin_theta);

    if constexpr (TShortestPath)
    {
        RkFloat    const sign = dot_result < 0.0F ? -1.0F : 1.0F;
        Quaternion const b    = Scale(in_rhs, Sin(static_cast<Radians>(sign * in_ratio) * theta) / sin_theta);

        return Add(a, b).Normalized();
    }

    Quaternion const b = Scale(in_rhs, Sin(static_cast<Radians>(in_ratio) * theta) / sin_theta);

    return Add(a, b).Normalized();
}

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