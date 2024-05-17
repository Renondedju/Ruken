
#pragma once

#pragma region Operators

template <>
constexpr Angle<EAngleUnit::Radian>::operator Angle<EAngleUnit::Degree>() const noexcept
{
    return Angle<EAngleUnit::Degree>(static_cast<RkFloat>(*this) * 180.0F / Constants<RkFloat>::pi);
}

template <>
constexpr Angle<EAngleUnit::Degree>::operator Angle<EAngleUnit::Radian>() const noexcept
{
    return Angle<EAngleUnit::Radian>(static_cast<RkFloat>(*this) * Constants<RkFloat>::pi / 180.0F);
}

constexpr Degrees operator"" _deg(RkLdouble const in_angle) noexcept
{
	return Degrees(static_cast<RkFloat>(in_angle));
}

constexpr Degrees operator"" _deg(RkULLInt const in_angle) noexcept
{
	return Degrees(static_cast<RkFloat>(in_angle));
}

constexpr Radians operator"" _rad(RkLdouble const in_angle) noexcept
{
	return Radians(static_cast<RkFloat>(in_angle));
}

constexpr Radians operator"" _rad(RkULLInt const in_angle) noexcept
{
	return Radians(static_cast<RkFloat>(in_angle));
}

#pragma endregion