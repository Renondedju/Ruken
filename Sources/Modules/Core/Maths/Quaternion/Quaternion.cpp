#include "Core/Maths/Math.hpp"
#include "Core/Maths/Plane.hpp"
#include "Core/Maths/Utility.hpp"
#include "Core/Maths/Trigonometry.hpp"
#include "Core/Maths/Quaternion/Quaternion.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

Quaternion::Quaternion(Vector3rad const& in_euler_rotation) noexcept
{
    Radians const half_x_angle = in_euler_rotation.x() / 2.0F;
    Radians const half_y_angle = in_euler_rotation.y() / 2.0F;
    Radians const half_z_angle = in_euler_rotation.z() / 2.0F;

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

Quaternion::Quaternion(Vector2rad const& in_euler_rotation) noexcept:
    Quaternion({in_euler_rotation.x(), in_euler_rotation.y(), 0_rad})
{}

Quaternion::Quaternion(Vector3m const& in_axis,
                       Radians  const  in_angle) noexcept
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

Quaternion& Quaternion::Normalize() noexcept
{
    RkFloat const length = Length();

    w /= length;
    x /= length;
    y /= length;
    z /= length;

    return *this;
}

Quaternion Quaternion::Normalized() const noexcept
{
    Quaternion result(*this);
    result.Normalize();
    return result;
}

Vector3m Quaternion::ToDirectionVector() const noexcept
{
    return *this * Constants<Vector3m>::forward;
}

Vector3rad Quaternion::Euler() const noexcept
{
    return Vector3rad {
        ArcTan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x*x + y*y)),
        ArcSin (2.0f * (w * y - z * x)),
        ArcTan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y*y + z*z)),
    };
}

Quaternion Quaternion::Inverted() const noexcept
{
    Quaternion new_quaternion {*this};

    new_quaternion.Invert();

    return new_quaternion;
}

#pragma endregion

#pragma region Static Methods

Quaternion Quaternion::LookAt(Vector3m const in_forward, Vector3m const in_up) noexcept
{
    Vector3m const forward {in_forward.Normalized()};
    Vector3m const right   {in_up.Cross(forward).Normalized()};
    Vector3m const up      {in_forward.Cross(right).Normalized()};

    Vector3m   const flat_direction {Plane3::ProjectVectorToPlane(forward, up).Normalized()};
    Quaternion const yaw            {up,    ArcCos(Clamp11(Constants<Vector3m>::forward.Dot(flat_direction)))};
    Quaternion const pitch          {right, ArcCos(Clamp11(flat_direction              .Dot(forward    )))};

    return yaw * pitch;
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