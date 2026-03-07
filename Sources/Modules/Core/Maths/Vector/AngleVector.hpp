#pragma once

#include "Types/Units/Angle/Angle.hpp"
#include "Build/Attributes.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

#include "Maths/Vector/Operations/VectorOperators.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorSlerp.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"

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

    union
	{
	    Angle<TAngleUnit> data[2];

	    struct
	    {
	        Angle<TAngleUnit> x;
			Angle<TAngleUnit> y;
	    };
		struct
	    {
	        Angle<TAngleUnit> pitch;
			Angle<TAngleUnit> yaw;
	    };
	};

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

	union
	{
		Angle<TAngleUnit> data[3];

		struct
		{
			Angle<TAngleUnit> x;
			Angle<TAngleUnit> y;
			Angle<TAngleUnit> z;
		};
		struct
		{
			Angle<TAngleUnit> pitch;
			Angle<TAngleUnit> yaw;
			Angle<TAngleUnit> roll;
		};
	};

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

	#pragma endregion
};

#pragma warning( pop )

using Vector2deg = Vector<2, Degrees>;
using Vector2rad = Vector<2, Radians>;
using Vector3deg = Vector<3, Degrees>;
using Vector3rad = Vector<3, Radians>;

END_RUKEN_NAMESPACE