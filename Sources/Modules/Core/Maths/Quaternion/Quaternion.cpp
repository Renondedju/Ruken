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

Quaternion::Quaternion(Vector3m const in_axis,
                       Radians  const in_angle) noexcept
{
    Vector3m const axis {in_axis.Normalized()};
    RkFloat  const sin  {Sin(in_angle / 2.0f)};

    w = Cos(in_angle / 2.0f);
    x = static_cast<RkFloat>(axis.x()) * sin;
    y = static_cast<RkFloat>(axis.y()) * sin;
    z = static_cast<RkFloat>(axis.z()) * sin;
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

Quaternion Quaternion::LookAt(Vector3m const in_direction, Vector3m const in_up) noexcept
{
    // Project in_direction onto the plane who's normal vector is in_up
    Vector3m   const projection     {in_up * in_direction.Dot(in_up)};
    Vector3m   const flat_direction {in_direction - projection};
    Quaternion const flat_rotation  {in_up.Cross(flat_direction), ArcCos(in_direction.Dot(flat_direction))};



    Quaternion const up_rotation    {};

    return flat_rotation * up_rotation;
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