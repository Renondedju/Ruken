#pragma once

#include "Core/Maths/Vector/Helper/VectorForward.hpp"
#include "Core/Maths/Vector/Operations/VectorLerp.hpp"
#include "Core/Maths/Vector/Operations/VectorMinMax.hpp"
#include "Core/Maths/Vector/Operations/VectorOperators.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma region Float Vectors

template <>
struct RUKEN_EMPTY_BASES Vector<2, RkFloat> final:
	VectorOperators<2, RkFloat>,
	VectorMinMax   <2, RkFloat>,
	VectorLerp     <2, RkFloat>
{
	RkFloat data[2];

	#pragma region Getters/Setters

	RkFloat&	   operator[](RkSize const in_offset)		noexcept { return data[in_offset]; }
	RkFloat const& operator[](RkSize const in_offset) const noexcept { return data[in_offset]; }

	RkFloat&	   x()       noexcept { return data[0]; }
	RkFloat const& x() const noexcept { return data[0]; }

	RkFloat&	   y()       noexcept { return data[1]; }
	RkFloat const& y() const noexcept { return data[1]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0.0f, 0.0f}
	{}

	constexpr Vector(RkFloat const in_x,
					 RkFloat const in_y) noexcept:
		data {in_x, in_y}
	{}

	#pragma endregion
};

template <>
struct RUKEN_EMPTY_BASES Vector<3, RkFloat> final:
	VectorOperators<3, RkFloat>,
	VectorMinMax   <3, RkFloat>,
	VectorLerp     <3, RkFloat>
{
	RkFloat data[3];

	#pragma region Getters/Setters

	RkFloat&	   operator[](RkSize const in_offset)		noexcept { return data[in_offset]; }
	RkFloat const& operator[](RkSize const in_offset) const noexcept { return data[in_offset]; }

	RkFloat&	   x()       noexcept { return data[0]; }
	RkFloat const& x() const noexcept { return data[0]; }

	RkFloat&	   y()       noexcept { return data[1]; }
	RkFloat const& y() const noexcept { return data[1]; }

	RkFloat&	   z()       noexcept { return data[2]; }
	RkFloat const& z() const noexcept { return data[2]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0.0f, 0.0f, 0.0f}
	{}

	constexpr Vector(RkFloat const in_x,
					 RkFloat const in_y,
					 RkFloat const in_z) noexcept:
		data {in_x, in_y, in_z}
	{}

	constexpr Vector(Vector<2, RkFloat> const& in_xy,
					 RkFloat		    const  in_z) noexcept:
		data {in_xy.x(), in_xy.y(), in_z}
	{}

	constexpr Vector(RkFloat			const  in_x,
					 Vector<2, RkFloat> const& in_yz) noexcept:
		data {in_x, in_yz.x(), in_yz.y()}
	{}

	#pragma endregion
};

template <>
struct RUKEN_EMPTY_BASES Vector<4, RkFloat> final:
	VectorOperators<4, RkFloat>,
	VectorMinMax   <4, RkFloat>,
	VectorLerp     <4, RkFloat>
{
	RkFloat data[4];

	#pragma region Getters/Setters

	RkFloat&	   operator[](RkSize const in_offset)		noexcept { return data[in_offset]; }
	RkFloat const& operator[](RkSize const in_offset) const noexcept { return data[in_offset]; }

	RkFloat&	   x()       noexcept { return data[0]; }
	RkFloat const& x() const noexcept { return data[0]; }

	RkFloat&	   y()       noexcept { return data[1]; }
	RkFloat const& y() const noexcept { return data[1]; }

	RkFloat&	   z()       noexcept { return data[2]; }
	RkFloat const& z() const noexcept { return data[2]; }

	RkFloat&	   w()       noexcept { return data[3]; }
	RkFloat const& w() const noexcept { return data[3]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0.0f, 0.0f, 0.0f, 0.0f}
	{}

	constexpr Vector(RkFloat const in_x,
					 RkFloat const in_y,
					 RkFloat const in_z,
					 RkFloat const in_w) noexcept:
		data {in_x, in_y, in_z, in_w}
	{}

	constexpr Vector(Vector<2, RkFloat> const& in_xy,
					 RkFloat			const  in_z,
					 RkFloat			const  in_w) noexcept:
		data {in_xy.x(), in_xy.y(), in_z, in_w}
	{}

	constexpr Vector(RkFloat			const  in_x,
					 Vector<2, RkFloat> const& in_yz,
					 RkFloat			const  in_w) noexcept:
		data {in_x, in_yz.x(), in_yz.y(), in_w}
	{}

	constexpr Vector(RkFloat			const  in_x,
					 RkFloat			const  in_y,
					 Vector<2, RkFloat> const& in_zw) noexcept:
		data {in_x, in_y, in_zw.x(), in_zw.y()}
	{}

	constexpr Vector(Vector<2, RkFloat> const& in_xy,
					 Vector<2, RkFloat> const& in_zw) noexcept:
		data {in_xy.x(), in_xy.y(), in_zw.x(), in_zw.y()}
	{}

	constexpr Vector(Vector<3, RkFloat> const& in_xyz,
					 RkFloat			const  in_w	) noexcept:
		data {in_xyz.x(), in_xyz.y(), in_xyz.z(), in_w}
	{}

	constexpr Vector(RkFloat			const  in_x,
					 Vector<3, RkFloat> const& in_yzw) noexcept:
		data {in_x, in_yzw.x(), in_yzw.y(), in_yzw.z()}
	{}

	#pragma endregion
};

using Vector2f = Vector<2, RkFloat>;
using Vector3f = Vector<3, RkFloat>;
using Vector4f = Vector<4, RkFloat>;

#pragma endregion

#pragma region Int Vectors

template <>
struct RUKEN_EMPTY_BASES Vector<2, RkInt> final:
	VectorOperators<2, RkInt>,
	VectorMinMax   <2, RkInt>,
	VectorLerp     <2, RkInt>
{
	RkInt data[2];

	#pragma region Getters/Setters

	RkInt&	     operator[](RkSize const in_offset)		  noexcept { return data[in_offset]; }
	RkInt const& operator[](RkSize const in_offset) const noexcept { return data[in_offset]; }

	RkInt&	     x()       noexcept { return data[0]; }
	RkInt const& x() const noexcept { return data[0]; }

	RkInt&	     y()       noexcept { return data[1]; }
	RkInt const& y() const noexcept { return data[1]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0, 0}
	{}

	constexpr Vector(RkInt const in_x,
					 RkInt const in_y) noexcept:
		data {in_x, in_y}
	{}

	#pragma endregion
};

template <>
struct RUKEN_EMPTY_BASES Vector<3, RkInt> final:
	VectorOperators<3, RkInt>,
	VectorMinMax   <3, RkInt>,
	VectorLerp     <3, RkInt>
{
	RkInt data[3];

	#pragma region Getters/Setters

	RkInt&	     operator[](RkSize const in_offset)		  noexcept { return data[in_offset]; }
	RkInt const& operator[](RkSize const in_offset) const noexcept { return data[in_offset]; }

	RkInt&	     x()       noexcept { return data[0]; }
	RkInt const& x() const noexcept { return data[0]; }

	RkInt&	     y()       noexcept { return data[1]; }
	RkInt const& y() const noexcept { return data[1]; }

	RkInt&	     z()       noexcept { return data[2]; }
	RkInt const& z() const noexcept { return data[2]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0, 0, 0}
	{}

	constexpr Vector(RkInt const in_x,
					 RkInt const in_y,
					 RkInt const in_z) noexcept:
		data {in_x, in_y, in_z}
	{}

	constexpr Vector(Vector<2, RkInt> const& in_xy,
					 RkInt			  const  in_z) noexcept:
		data {in_xy.x(), in_xy.y(), in_z}
	{}

	constexpr Vector(RkInt			 const  in_x,
					 Vector<2, RkInt> const& in_yz) noexcept:
		data {in_x, in_yz.x(), in_yz.y()}
	{}

	#pragma endregion
};

template <>
struct RUKEN_EMPTY_BASES Vector<4, RkInt> final:
	VectorOperators<4, RkInt>,
	VectorMinMax   <4, RkInt>,
	VectorLerp     <4, RkInt>
{
	RkInt data[4];

	#pragma region Getters/Setters

	RkInt&	     operator[](RkSize const in_offset)		  noexcept { return data[in_offset]; }
	RkInt const& operator[](RkSize const in_offset) const noexcept { return data[in_offset]; }

	RkInt&	     x()       noexcept { return data[0]; }
	RkInt const& x() const noexcept { return data[0]; }

	RkInt&	     y()       noexcept { return data[1]; }
	RkInt const& y() const noexcept { return data[1]; }

	RkInt&	     z()       noexcept { return data[2]; }
	RkInt const& z() const noexcept { return data[2]; }

	RkInt&	     w()       noexcept { return data[3]; }
	RkInt const& w() const noexcept { return data[3]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0, 0, 0, 0}
	{}

	constexpr Vector(RkInt const in_x,
					 RkInt const in_y,
					 RkInt const in_z,
					 RkInt const in_w) noexcept:
		data {in_x, in_y, in_z, in_w}
	{}

	constexpr Vector(Vector<2, RkInt> const& in_xy,
					 RkInt			  const  in_z,
					 RkInt			  const  in_w) noexcept:
		data {in_xy.x(), in_xy.y(), in_z, in_w}
	{}

	constexpr Vector(RkInt			  const  in_x,
					 Vector<2, RkInt> const& in_yz,
					 RkInt			  const  in_w) noexcept:
		data {in_x, in_yz.x(), in_yz.y(), in_w}
	{}

	constexpr Vector(RkInt			  const  in_x,
					 RkInt			  const  in_y,
					 Vector<2, RkInt> const& in_zw) noexcept:
		data {in_x, in_y, in_zw.x(), in_zw.y()}
	{}

	constexpr Vector(Vector<2, RkInt> const& in_xy,
					 Vector<2, RkInt> const& in_zw) noexcept:
		data {in_xy.x(), in_xy.y(), in_zw.x(), in_zw.y()}
	{}

	constexpr Vector(Vector<3, RkInt> const& in_xyz,
					 RkInt			  const  in_w	) noexcept:
		data {in_xyz.x(), in_xyz.y(), in_xyz.z(), in_w}
	{}

	constexpr Vector(RkInt			  const  in_x,
					 Vector<3, RkInt> const& in_yzw) noexcept:
		data {in_x, in_yzw.x(), in_yzw.y(), in_yzw.z()}
	{}

	#pragma endregion
};

using Vector2i = Vector<2, RkInt>;
using Vector3i = Vector<3, RkInt>;
using Vector4i = Vector<4, RkInt>;

#pragma endregion

END_RUKEN_NAMESPACE