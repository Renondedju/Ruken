#pragma once

#include "Build/Attributes.hpp"
#include "Types/Units/Distance/Distance.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

#include "Maths/Vector/Operations/VectorNormalization.hpp"
#include "Maths/Vector/Operations/VectorOperators.hpp"
#include "Maths/Vector/Operations/VectorLength.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorCross.hpp"
#include "Maths/Vector/Operations/VectorSlerp.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"
#include "Maths/Vector/Operations/VectorDot.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning( push )
#pragma warning( disable:4201 )

template <RkSize TSize, EDistanceUnit TDistanceUnit>
struct DistanceVectorOperations:
	VectorNormalization<TSize, Distance<TDistanceUnit>>,
	VectorOperators    <TSize, Distance<TDistanceUnit>>,
	VectorMinMax       <TSize, Distance<TDistanceUnit>>,
	VectorLength       <TSize, Distance<TDistanceUnit>>,
	VectorCross        <TSize, Distance<TDistanceUnit>>,
	VectorSlerp        <TSize, Distance<TDistanceUnit>>,
	VectorLerp         <TSize, Distance<TDistanceUnit>>,
	VectorDot          <TSize, Distance<TDistanceUnit>>
{};

/**
 * Generic distance vector.
 * @tparam TSize Dimensionality of the vector.
 * @tparam TDistanceUnit Distance unit of the vector.
 */
template <RkSize TSize, EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES Vector<TSize, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<TSize, TDistanceUnit>
{
	#pragma region Members

	Distance<TDistanceUnit> data[TSize] {Distance<TDistanceUnit>(0)};

	#pragma endregion
};

#pragma region Specializations

/// @brief Two-dimensional distance vector specialization
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES Vector<2, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<2, TDistanceUnit>
{
	#pragma region Members

	union
	{
		Distance<TDistanceUnit> data[2];

		struct
		{
			Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
		};
		struct
		{
			Distance<TDistanceUnit> width;
			Distance<TDistanceUnit> height;
		};
	};

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {Distance<TDistanceUnit>(0),
			  Distance<TDistanceUnit>(0)}
	{}

	constexpr Vector(Distance<TDistanceUnit> const in_x,
					 Distance<TDistanceUnit> const in_y) noexcept:
		data {in_x, in_y}
	{}

	#pragma endregion
};

/// @brief Three-dimensional distance vector specialization
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES Vector<3, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<3, TDistanceUnit>
{
    #pragma region Members

    union
	{
	    Distance<TDistanceUnit> data[3];

	    struct
	    {
	        Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
			Distance<TDistanceUnit> z;
	    };
		struct
	    {
	        Distance<TDistanceUnit> width;
			Distance<TDistanceUnit> height;
			Distance<TDistanceUnit> depth;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Distance<TDistanceUnit>(0),
	          Distance<TDistanceUnit>(0),
	          Distance<TDistanceUnit>(0)}
	{}

    constexpr Vector(Distance<TDistanceUnit> const in_x,
                     Distance<TDistanceUnit> const in_y,
                     Distance<TDistanceUnit> const in_z) noexcept:
		data {in_x, in_y, in_z}
	{}

    #pragma endregion
};

/// @brief Four-dimensional distance vector specialization
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES Vector<4, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<4, TDistanceUnit>
{
	#pragma region Members

	union
	{
		Distance<TDistanceUnit> data[3];

		struct
		{
			Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
			Distance<TDistanceUnit> z;
			Distance<TDistanceUnit> w;
		};
	};

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {Distance<TDistanceUnit>(0),
			  Distance<TDistanceUnit>(0),
			  Distance<TDistanceUnit>(0),
		      Distance<TDistanceUnit>(0)}
	{}

	constexpr Vector(Distance<TDistanceUnit> const in_x,
					 Distance<TDistanceUnit> const in_y,
					 Distance<TDistanceUnit> const in_z,
					 Distance<TDistanceUnit> const in_w) noexcept:
		data {in_x, in_y, in_z, in_w}
	{}

	#pragma endregion
};

#pragma endregion

#pragma warning( pop )

using Vector2cm = Vector<2, Centimeters>;
using Vector2m  = Vector<2, Meters>;
using Vector2km = Vector<2, Kilometers>;

using Vector3cm = Vector<3, Centimeters>;
using Vector3m  = Vector<3, Meters>;
using Vector3km = Vector<3, Kilometers>;

using Vector4cm = Vector<4, Centimeters>;
using Vector4m  = Vector<4, Meters>;
using Vector4km = Vector<4, Kilometers>;

#pragma region Constants

template <EDistanceUnit TDistanceUnit>
struct Constants<Vector<2, Distance<TDistanceUnit>>>
{
	static constexpr Vector<2, Distance<TDistanceUnit>> one {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> zero {
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> right {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> left {
		Distance<TDistanceUnit>(-1.0F),
		Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> up {
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> down {
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F)
	};
};

template <EDistanceUnit TDistanceUnit>
struct Constants<Vector<3, Distance<TDistanceUnit>>>
{
	static constexpr Vector<3, Distance<TDistanceUnit>> one {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> zero {
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> right {
		Distance<TDistanceUnit>(1.0F),
        Distance<TDistanceUnit>(0.0F),
        Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> left {
		Distance<TDistanceUnit>(-1.0F),
        Distance<TDistanceUnit>( 0.0F),
        Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> up {
        Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F),
        Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> down {
        Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F),
        Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> forward {
        Distance<TDistanceUnit>(0.0F),
        Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<3, Distance<TDistanceUnit>> backward {
        Distance<TDistanceUnit>( 0.0F),
        Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F)
	};
};

template <EDistanceUnit TDistanceUnit>
struct Constants<Vector<4, Distance<TDistanceUnit>>>
{
	static constexpr Vector<4, Distance<TDistanceUnit>> one {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> zero {
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F),
		Distance<TDistanceUnit>(.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> right {
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> left {
		Distance<TDistanceUnit>(-1.0F),
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> up {
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> down {
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F),
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> forward {
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F),
		Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<4, Distance<TDistanceUnit>> backward {
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F),
		Distance<TDistanceUnit>( 0.0F)
	};
};

#pragma endregion

END_RUKEN_NAMESPACE