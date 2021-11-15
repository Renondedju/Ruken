
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