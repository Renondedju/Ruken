#pragma once

#include "Core/Maths/Matrix/Matrix.hpp"
#include "Core/Maths/Vector/AngleVector.hpp"
#include "Core/Maths/Vector/DistanceVector.hpp"
#include "Core/Maths/Quaternion/Quaternion.hpp"

BEGIN_RUKEN_NAMESPACE

struct SphericalCoord
{
	#pragma region Lifetime

	explicit SphericalCoord() = default;
	explicit SphericalCoord(Vector2rad const& in_angles   ) noexcept;
	explicit SphericalCoord(Vector3m   const& in_direction) noexcept;

	SphericalCoord		      (SphericalCoord const&) = default;
	SphericalCoord			  (SphericalCoord&&)	  = default;
	SphericalCoord& operator= (SphericalCoord const&) = default;
	SphericalCoord& operator= (SphericalCoord&&)	  = default;
	~SphericalCoord()								  = default;

	#pragma endregion

	#pragma region Members

	Vector2rad angles {};

	#pragma endregion

	#pragma region Methods
	
	RkVoid		   Normalize      ()	   noexcept;
	SphericalCoord Normalized     () const noexcept;
	Vector3m	   DirectionVector() const noexcept;
	Matrix3x3	   AsMatrix		  () const noexcept;
	Quaternion	   AsQuaternion   () const noexcept;

	/**
	 * Linear interpolation.
	 * @param in_from From rotation.
	 * @param in_to To rotation.
	 * @param in_ratio Interpolation ratio.
	 * @return Interpolated rotation.
	 */
	static SphericalCoord Lerp(
		SphericalCoord const& in_from,
		SphericalCoord const& in_to,
		RkFloat				  in_ratio) noexcept;

	#pragma endregion

	#pragma region Operators

	SphericalCoord  operator+ (SphericalCoord const& in_other) const noexcept;
	SphericalCoord  operator- (SphericalCoord const& in_other) const noexcept;
	SphericalCoord& operator+=(SphericalCoord const& in_other) noexcept;
	SphericalCoord& operator-=(SphericalCoord const& in_other) noexcept;
	SphericalCoord  operator* (RkFloat in_scalar) const noexcept;
	SphericalCoord  operator/ (RkFloat in_scalar) const noexcept;
	SphericalCoord& operator*=(RkFloat in_scalar) noexcept;
	SphericalCoord& operator/=(RkFloat in_scalar) noexcept;


	#pragma endregion
};

END_RUKEN_NAMESPACE