#include "Core/Maths/SphericalCoord.hpp"

USING_RUKEN_NAMESPACE

SphericalCoord::SphericalCoord(Vector2rad const& in_angles) noexcept:
	angles {in_angles}
{}

SphericalCoord::SphericalCoord(Vector3m const& in_direction) noexcept:
	angles {
		ArcCos (static_cast<RkFloat>(in_direction.y() / in_direction.Length())) - Constants<Radians>::pi_2,
		ArcTan2(static_cast<RkFloat>(in_direction.x()), static_cast<RkFloat>(in_direction.z()))
	}
{}

RkVoid SphericalCoord::Normalize() noexcept
{
	angles.x() = static_cast<Radians>(Mod(static_cast<RkFloat>(angles.x()), static_cast<RkFloat>(Constants<Radians>::pi) * 2.0f));
	angles.y() = static_cast<Radians>(Mod(static_cast<RkFloat>(angles.y()), static_cast<RkFloat>(Constants<Radians>::pi) * 2.0f));
}

SphericalCoord SphericalCoord::Normalized() const noexcept
{
	SphericalCoord result(*this);
	result.Normalize();

	return result;
}

Vector3m SphericalCoord::DirectionVector() const noexcept
{
	return Vector3m {
		static_cast<Meters>( Cos(angles.x()) * Sin(angles.y())),
		static_cast<Meters>(-Sin(angles.x())),
		static_cast<Meters>( Cos(angles.x()) * Cos(angles.y())),
	}.Normalized();
}

Matrix3x3 SphericalCoord::AsMatrix() const noexcept
{
	return Matrix3x3::RotationMatrixX(angles.x()) *
		   Matrix3x3::RotationMatrixY(angles.y());
}

Quaternion SphericalCoord::AsQuaternion() const noexcept
{
	return
		Quaternion({0_rad, angles.y()}) *
		Quaternion({angles.x(), 0_rad});
}

SphericalCoord SphericalCoord::Lerp(
	SphericalCoord const& in_from,
	SphericalCoord const& in_to,
	RkFloat		   const  in_ratio) noexcept
{
	auto const from {in_from.Normalized()};
	auto	   to   {in_to  .Normalized()};

	// Computing the shortest path
	if (Absolute(from.angles.x() - from.angles.x()) > Constants<Radians>::pi)
		to.angles.x() += CopySign(Constants<Radians>::pi_times_2, from.angles.x());

	if (Absolute(from.angles.y() - from.angles.y()) > Constants<Radians>::pi)
		to.angles.y() += CopySign(Constants<Radians>::pi_times_2, from.angles.y());

	return in_from * (1.0f - in_ratio) + in_to * in_ratio;
}

SphericalCoord SphericalCoord::operator+(SphericalCoord const& in_other) const noexcept
{
	return SphericalCoord {angles + in_other.angles};
}

SphericalCoord SphericalCoord::operator-(SphericalCoord const& in_other) const noexcept
{
	return SphericalCoord {angles - in_other.angles};
}

SphericalCoord& SphericalCoord::operator+=(SphericalCoord const& in_other) noexcept
{
	angles += in_other.angles;

	return *this;
}


SphericalCoord& SphericalCoord::operator-=(SphericalCoord const& in_other) noexcept
{
	angles -= in_other.angles;

	return *this;
}

SphericalCoord SphericalCoord::operator*(RkFloat const in_scalar) const noexcept
{
	return SphericalCoord {angles * in_scalar};
}

SphericalCoord SphericalCoord::operator/(RkFloat const in_scalar) const noexcept
{
	return SphericalCoord {angles / in_scalar};
}

SphericalCoord& SphericalCoord::operator*=(RkFloat const in_scalar) noexcept
{
	angles *= in_scalar;

	return *this;
}

SphericalCoord& SphericalCoord::operator/=(RkFloat const in_scalar) noexcept
{
	angles /= in_scalar;

	return *this;
}