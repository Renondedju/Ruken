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

#ifdef RUKEN_COMPILER_MSVC
	#pragma warning( push )
	#pragma warning( disable:4201 )
#endif

template <RkSize TSize, EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES DistanceVectorOperations:
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
struct RUKEN_EMPTY_BASES alignas(16) Vector<TSize, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<TSize, TDistanceUnit>
{
	#pragma region Members

	Distance<TDistanceUnit> data[TSize] {Distance<TDistanceUnit>(0)};

	#pragma endregion
};

#pragma region Specializations

/// @brief Two-dimensional distance vector specialization
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES alignas(8) Vector<2, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<2, TDistanceUnit>
{
	Distance<TDistanceUnit> data[2];

	#pragma region Getters/Setters

	Distance<TDistanceUnit>&	   operator[](RkSize in_offset)		  noexcept { return data[in_offset]; }
	Distance<TDistanceUnit> const& operator[](RkSize in_offset) const noexcept { return data[in_offset]; }

	Distance<TDistanceUnit>&	   x()       noexcept { return data[0]; }
	Distance<TDistanceUnit> const& x() const noexcept { return data[0]; }

	Distance<TDistanceUnit>&	   y()       noexcept { return data[1]; }
	Distance<TDistanceUnit> const& y() const noexcept { return data[1]; }

	Distance<TDistanceUnit>&	   Width ()       noexcept { return data[0]; }
	Distance<TDistanceUnit> const& Width () const noexcept { return data[0]; }

	Distance<TDistanceUnit>&	   Height()       noexcept { return data[1]; }
	Distance<TDistanceUnit> const& Height() const noexcept { return data[1]; }

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
struct RUKEN_EMPTY_BASES alignas(16) Vector<3, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<3, TDistanceUnit>
{
	Distance<TDistanceUnit> data[3];

	#pragma region Getters/Setters

	Distance<TDistanceUnit>&	   operator[](RkSize in_offset)		  noexcept { return data[in_offset]; }
	Distance<TDistanceUnit> const& operator[](RkSize in_offset) const noexcept { return data[in_offset]; }

	Distance<TDistanceUnit>&	   x()       noexcept { return data[0]; }
	Distance<TDistanceUnit> const& x() const noexcept { return data[0]; }

	Distance<TDistanceUnit>&	   y()       noexcept { return data[1]; }
	Distance<TDistanceUnit> const& y() const noexcept { return data[1]; }

	Distance<TDistanceUnit>&	   z()       noexcept { return data[2]; }
	Distance<TDistanceUnit> const& z() const noexcept { return data[2]; }

	Distance<TDistanceUnit>&	   Width ()       noexcept { return data[0]; }
	Distance<TDistanceUnit> const& Width () const noexcept { return data[0]; }

	Distance<TDistanceUnit>&	   Height()       noexcept { return data[1]; }
	Distance<TDistanceUnit> const& Height() const noexcept { return data[1]; }

	Distance<TDistanceUnit>&	   Depth ()       noexcept { return data[2]; }
	Distance<TDistanceUnit> const& Depth () const noexcept { return data[2]; }

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

	constexpr Vector(Vector<2, Distance<TDistanceUnit>> const& in_xy,
					 Distance<TDistanceUnit>			const  in_z) noexcept:
		data {in_xy.x(), in_xy.y(), in_z}
	{}

	constexpr Vector(Distance<TDistanceUnit>			const  in_x,
					 Vector<2, Distance<TDistanceUnit>> const& in_yz) noexcept:
		data {in_x, in_yz.x(), in_yz.y()}
	{}

    #pragma endregion
};

/// @brief Four-dimensional distance vector specialization
template <EDistanceUnit TDistanceUnit>
struct RUKEN_EMPTY_BASES alignas(16) Vector<4, Distance<TDistanceUnit>> final:
	DistanceVectorOperations<4, TDistanceUnit>
{
	Distance<TDistanceUnit> data[4];

	#pragma region Getters/Setters

	Distance<TDistanceUnit>&	   operator[](RkSize in_offset)		  noexcept { return data[in_offset]; }
	Distance<TDistanceUnit> const& operator[](RkSize in_offset) const noexcept { return data[in_offset]; }

	Distance<TDistanceUnit>&	   x()       noexcept { return data[0]; }
	Distance<TDistanceUnit> const& x() const noexcept { return data[0]; }

	Distance<TDistanceUnit>&	   y()       noexcept { return data[1]; }
	Distance<TDistanceUnit> const& y() const noexcept { return data[1]; }

	Distance<TDistanceUnit>&	   z()       noexcept { return data[2]; }
	Distance<TDistanceUnit> const& z() const noexcept { return data[2]; }

	Distance<TDistanceUnit>&	   w()       noexcept { return data[3]; }
	Distance<TDistanceUnit> const& w() const noexcept { return data[3]; }

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

	constexpr Vector(Vector<2, Distance<TDistanceUnit>> const& in_xy,
					 Distance<TDistanceUnit>			const  in_z,
					 Distance<TDistanceUnit>			const  in_w) noexcept:
		data {in_xy.x(), in_xy.y(), in_z, in_w}
	{}

	constexpr Vector(Distance<TDistanceUnit>			const  in_x,
					 Vector<2, Distance<TDistanceUnit>> const& in_yz,
					 Distance<TDistanceUnit>			const  in_w) noexcept:
		data {in_x, in_yz.x(), in_yz.y(), in_w}
	{}

	constexpr Vector(Distance<TDistanceUnit>			const  in_x,
					 Distance<TDistanceUnit>			const  in_y,
					 Vector<2, Distance<TDistanceUnit>> const& in_zw) noexcept:
		data {in_x, in_y, in_zw.x(), in_zw.y()}
	{}

	constexpr Vector(Vector<2, Distance<TDistanceUnit>> const& in_xy,
					 Vector<2, Distance<TDistanceUnit>> const& in_zw) noexcept:
		data {in_xy.x(), in_xy.y(), in_zw.x(), in_zw.y()}
	{}

	constexpr Vector(Vector<3, Distance<TDistanceUnit>> const& in_xyz,
					 Distance<TDistanceUnit>			const  in_w	) noexcept:
		data {in_xyz.x(), in_xyz.y(), in_xyz.z(), in_w}
	{}

	constexpr Vector(Distance<TDistanceUnit>			const  in_x,
					 Vector<3, Distance<TDistanceUnit>> const& in_yzw) noexcept:
		data {in_x, in_yzw.x(), in_yzw.y(), in_yzw.z()}
	{}

	#pragma endregion

	#pragma region Methods

	Vector<3, Distance<TDistanceUnit>> Homogenize() const noexcept
	{
		return Vector<3, Distance<TDistanceUnit>>{
			data[0] / data[3],
			data[1] / data[3],
			data[2] / data[3],
		};
	}

	#pragma endregion
};

#pragma endregion

#ifdef RUKEN_COMPILER_MSVC
	#pragma warning( pop )
#endif

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