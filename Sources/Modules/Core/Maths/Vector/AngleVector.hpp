#pragma once

#include "Core/Build/Attributes.hpp"
#include "Core/Types/Units/Angle/Angle.hpp"
#include "Core/Maths/Vector/Helper/VectorForward.hpp"
#include "Core/Maths/Vector/Operations/VectorFormatter.hpp"
#include "Core/Maths/Vector/Operations/VectorOperators.hpp"
#include "Core/Maths/Vector/Operations/VectorMinMax.hpp"
#include "Core/Maths/Vector/Operations/VectorSlerp.hpp"
#include "Core/Maths/Vector/Operations/VectorLerp.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning( push )
#pragma warning( disable:4201 )

template <RkSize TSize, EAngleUnit TAngleUnit>
struct RUKEN_EMPTY_BASES AngleVectorOperations:
	VectorOperators<TSize, Angle<TAngleUnit>>,
	VectorMinMax   <TSize, Angle<TAngleUnit>>,
	VectorSlerp    <TSize, Angle<TAngleUnit>>,
	VectorLerp     <TSize, Angle<TAngleUnit>>
{};

/**
 * Generic angle vector.
 * @tparam TSize Dimensionality of the vector.
 * @tparam TAngleUnit Angle unit of the vector.
 */
template <RkSize TSize, EAngleUnit TAngleUnit>
struct RUKEN_EMPTY_BASES Vector<TSize, Angle<TAngleUnit>> final:
	AngleVectorOperations<TSize, TAngleUnit>
{
	#pragma region Members

	Angle<TAngleUnit> data[TSize] {Angle<TAngleUnit>(0)};

	#pragma endregion
};

/// @brief Two-dimensional angle vector
template <EAngleUnit TAngleUnit>
struct RUKEN_EMPTY_BASES Vector<2, Angle<TAngleUnit>> final:
	AngleVectorOperations<2, TAngleUnit>
{
    #pragma region Members

	Angle<TAngleUnit> data[2];

    #pragma endregion

	#pragma region Getters/Setters

	Angle<TAngleUnit>&	     operator[](RkSize in_offset)		  noexcept { return data[in_offset]; }
	Angle<TAngleUnit> const& operator[](RkSize in_offset) const noexcept { return data[in_offset]; }

	Angle<TAngleUnit>&	     x()       noexcept { return data[0]; }
	Angle<TAngleUnit> const& x() const noexcept { return data[0]; }

	Angle<TAngleUnit>&	     y()       noexcept { return data[1]; }
	Angle<TAngleUnit> const& y() const noexcept { return data[1]; }

	Angle<TAngleUnit>&	     Pitch()       noexcept { return data[0]; }
	Angle<TAngleUnit> const& Pitch() const noexcept { return data[0]; }

	Angle<TAngleUnit>&	     Yaw()       noexcept { return data[1]; }
	Angle<TAngleUnit> const& Yaw() const noexcept { return data[1]; }

	#pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Angle<TAngleUnit>(0), Angle<TAngleUnit>(0)}
	{}

    constexpr Vector(Angle<TAngleUnit> const in_pitch, Angle<TAngleUnit> const in_yaw) noexcept:
		data {in_pitch, in_yaw}
	{}

    #pragma endregion
};

/// @brief Three-dimensional angle vector
template <EAngleUnit TAngleUnit>
struct RUKEN_EMPTY_BASES Vector<3, Angle<TAngleUnit>> final:
	AngleVectorOperations<3, TAngleUnit>
{
	#pragma region Members

	Angle<TAngleUnit> data[3];

	#pragma endregion

	#pragma region Getters/Setters

	#pragma region Getters/Setters

	Angle<TAngleUnit>&	     operator[](RkSize in_offset)		  noexcept { return data[in_offset]; }
	Angle<TAngleUnit> const& operator[](RkSize in_offset) const noexcept { return data[in_offset]; }

	Angle<TAngleUnit>&	     x()       noexcept { return data[0]; }
	Angle<TAngleUnit> const& x() const noexcept { return data[0]; }

	Angle<TAngleUnit>&	     y()       noexcept { return data[1]; }
	Angle<TAngleUnit> const& y() const noexcept { return data[1]; }

	Angle<TAngleUnit>&	     z()       noexcept { return data[2]; }
	Angle<TAngleUnit> const& z() const noexcept { return data[2]; }

	Angle<TAngleUnit>&	     Pitch()       noexcept { return data[0]; }
	Angle<TAngleUnit> const& Pitch() const noexcept { return data[0]; }

	Angle<TAngleUnit>&	     Yaw()       noexcept { return data[1]; }
	Angle<TAngleUnit> const& Yaw() const noexcept { return data[1]; }

	Angle<TAngleUnit>&	     Roll()       noexcept { return data[2]; }
	Angle<TAngleUnit> const& Roll() const noexcept { return data[2]; }

	#pragma endregion

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {Angle<TAngleUnit>(0),
			  Angle<TAngleUnit>(0),
			  Angle<TAngleUnit>(0)}
	{}

	constexpr Vector(Angle<TAngleUnit> const in_pitch,
					 Angle<TAngleUnit> const in_yaw,
					 Angle<TAngleUnit> const in_roll) noexcept:
		data {in_pitch, in_yaw, in_roll}
	{}

	constexpr Vector(Vector<2, Angle<TAngleUnit>> const& in_xy,
					 Angle<TAngleUnit>			  const  in_z) noexcept:
		data {in_xy.x(), in_xy.y(), in_z}
	{}

	constexpr Vector(Angle<TAngleUnit>			  const  in_x,
					 Vector<2, Angle<TAngleUnit>> const& in_yz) noexcept:
		data {in_x, in_yz.x(), in_yz.y()}
	{}

	#pragma endregion
};

#pragma warning( pop )

using Vector2deg = Vector<2, Degrees>;
using Vector2rad = Vector<2, Radians>;
using Vector3deg = Vector<3, Degrees>;
using Vector3rad = Vector<3, Radians>;

END_RUKEN_NAMESPACE

// Format specialization
template <RkSize TDimensions, RUKEN_NAMESPACE::EAngleUnit TAngleUnit, typename TChar>
struct std::formatter<RUKEN_NAMESPACE::Vector<TDimensions, RUKEN_NAMESPACE::Angle<TAngleUnit>>, TChar> :
	RUKEN_NAMESPACE::VectorFormatter<TDimensions, RUKEN_NAMESPACE::Angle<TAngleUnit>, TChar>
{};