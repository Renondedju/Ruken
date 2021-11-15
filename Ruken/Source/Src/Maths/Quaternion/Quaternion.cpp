
#include "Maths/Math.hpp"
#include "Maths/Utility.hpp"
#include "Maths/Trigonometry.hpp"
#include "Maths/Quaternion/Quaternion.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Quaternion::Quaternion(Radians const in_angle_x,
                       Radians const in_angle_y,
                       Radians const in_angle_z) noexcept
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

#pragma endregion

#pragma region Methods

RkFloat Quaternion::Length() const noexcept
{
    return Sqrt(SqrLength());
}

Quaternion& Quaternion::Normalized() noexcept
{
    RkFloat const length = Length();

    w /= length;
    x /= length;
    y /= length;
    z /= length;

    return *this;
}

#pragma endregion

#pragma region Static Methods

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

        return Quaternion::Add(a, b).Normalized();
    }

    Quaternion const b = Quaternion::Scale(in_rhs, Sin(static_cast<Radians>(in_ratio) * theta) / sin_theta);

    return Quaternion::Add(a, b).Normalized();
}

RkFloat Quaternion::Dot(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept
{
    return in_lhs.w * in_rhs.w + in_lhs.x * in_rhs.x + in_lhs.y * in_rhs.y + in_lhs.z * in_rhs.z;
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

#pragma endregion