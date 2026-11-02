#pragma once

#include "Core/Maths/Matrix/Matrix.hpp"
#include "Core/Debug/Assert.hpp"
#include "Maths/Matrix/Matrix.hpp"

BEGIN_RUKEN_NAMESPACE
	template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns>::Matrix() noexcept:
	data {}
{
	// Init with identity if square matrix
	if constexpr (is_square)
		for (RkSize index {0ULL}; index < TRows; ++index)
			data[index + index * TRows] = 1.0F;
}

template<RkSize TRows, RkSize TColumns>
template<typename ... TTypes> requires (sizeof...(TTypes) == TRows * TColumns)
constexpr Matrix<TRows, TColumns>::Matrix(TTypes... in_values) noexcept:
	data { static_cast<RkFloat>(in_values)... }
{}

#pragma region Static Methods

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::LookAtMatrix(
	Vector3m const& in_from,
	Vector3m const& in_to,
	Vector3m const& in_up) noexcept requires ((TRows == 3 || TRows == 4) && TColumns == 4)
{
	Vector3m const forward {(in_to - in_from)     .Normalized()};
	Vector3m const right   {in_up  .Cross(forward).Normalized()};
	Vector3m const up      {forward.Cross(right)  .Normalized()};

	return Matrix<4, 4> {
		 right.x(),           up.x(),           forward.x(), 		  0.0f,
		 right.y(),           up.y(),           forward.y(), 		  0.0f,
		 right.z(),           up.z(),           forward.z(), 		  0.0f,
		-right.Dot(in_from), -up.Dot(in_from), -forward.Dot(in_from), 1.0f
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::ModelMatrix(
	Vector3m   const& in_position,
	Quaternion const& in_rotation,
	Vector3m   const& in_scale) noexcept requires (TRows == 4 && TColumns == 4)
{
	return Matrix::TranslationMatrix(in_position) *
	       Matrix::RotationMatrix3D (in_rotation) *
	       Matrix::ScaleMatrix		(in_scale)    ;
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::ModelMatrix(
	Vector3m   const& in_position,
	Quaternion const& in_rotation) noexcept requires (TRows == 4 && TColumns == 4)
{
	return Matrix::RotationMatrix3D (in_rotation) *
		   Matrix::TranslationMatrix(in_position);
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::PerspectiveProjection(
	Radians const in_fov,
	RkFloat const in_aspect,
	Meters  const in_near,
	Meters  const in_far) noexcept requires (TRows == 4 && TColumns == 4)
{
	// https://www.kdab.com/projection-matrices-with-vulkan-part-1/
	// https://www.kdab.com/projection-matrices-with-vulkan-part-2/
	// Post view correction is used. This matrix is specific to vulkan.
	RkFloat const fov_tangent {Tan(in_fov / 2.0F)};
	RkFloat const far         {-in_far};
	RkFloat const near        {-in_near};
	RkFloat const f_n         {far - near};

	return Matrix<4, 4> {
		(1.0f/in_aspect) / fov_tangent, 0.0F,  0.0F, 0.0F,
		0.0F, 1.0f / fov_tangent, 0.0F, 0.0F,
		0.0F, 0.0F, -far / f_n,          1.0F,
		0.0F, 0.0F, -(far * near) / f_n, 0.0F
	} * ClipSpace();
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::OrthogonalProjection(
	Meters const in_left,
	Meters const in_right,
	Meters const in_bottom,
	Meters const in_top,
	Meters const in_near,
	Meters const in_far) noexcept requires (TRows == 4 && TColumns == 4)
{
	// https://www.kdab.com/projection-matrices-with-vulkan-part-1/
	// https://www.kdab.com/projection-matrices-with-vulkan-part-2/
	// Post view correction is used. This matrix is specific to vulkan.
	auto const top {-in_top};
	auto const bot {-in_bottom};
	auto const rl  {in_right - in_left};
	auto const bt  {bot      - top };
	auto const nf  {in_near  - in_far };

	return Matrix<4, 4> {
		2.0_m / rl, 0.0F, 0.0F, 0.0F,
		0.0F, 2.0_m / bt, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0_m / nf, 0.0F,

		-(in_right  + in_left) / rl,
		-(bot + top) / bt,
		in_near      / nf,
		1.0F
	} * ClipSpace();
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::ClipSpace() noexcept requires (TRows == 4 && TColumns == 4)
{
	return Matrix<4, 4> {
		1.0F,  0.0F,  0.0F,  0.0F,
		0.0F, -1.0F,  0.0F,  0.0F,
		0.0F,  0.0F, -1.0F,  0.0F,
		0.0F,  0.0F,  0.0F,  1.0F
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrix3D(Quaternion const& in_rotation) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const qxx {in_rotation.x * in_rotation.x};
	RkFloat const qyy {in_rotation.y * in_rotation.y};
	RkFloat const qzz {in_rotation.z * in_rotation.z};
	RkFloat const qxz {in_rotation.x * in_rotation.z};
	RkFloat const qxy {in_rotation.x * in_rotation.y};
	RkFloat const qyz {in_rotation.y * in_rotation.z};
	RkFloat const qwx {in_rotation.w * in_rotation.x};
	RkFloat const qwy {in_rotation.w * in_rotation.y};
	RkFloat const qwz {in_rotation.w * in_rotation.z};

	return Matrix3x3 {
		1.0f - 2.0f * (qyy + qzz),        2.0f * (qxy + qwz),         2.0f * (qxz - qwy),
		       2.0f * (qxy - qwz), 1.0f - 2.0f * (qxx + qzz),         2.0f * (qyz + qwx),
		       2.0f * (qxz + qwy),        2.0f * (qyz - qwx),  1.0f - 2.0f * (qxx + qyy),
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrix3D(Vector3rad const& in_euler_angles) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	return RotationMatrix3D(Quaternion {in_euler_angles});
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrix3D(
	Vector3m const& in_axis,
	Radians  const  in_angle) noexcept requires (TRows >= 3 && TColumns >= 3)
{
	return RotationMatrix3D(Quaternion {in_axis, in_angle});
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrixX(Radians const in_angle) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const cos = Cos(in_angle);
	RkFloat const sin = Sin(in_angle);

	return Matrix<3, 3> {
		1.0F,  0.0F,  0.0F,
		0.0F,  cos,   sin,
		0.0F, -sin,   cos
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrixY(Radians const in_angle) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const cos = Cos(in_angle);
	RkFloat const sin = Sin(in_angle);

	return Matrix<3, 3> {
		cos ,  0.0F, -sin,
		0.0F,  1.0F,  0.0F,
		sin ,  0.0F,  cos
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrixZ(Radians const in_angle) noexcept
	requires (TRows >= 2 && TColumns >= 2)
{
	RkFloat const cos = Cos(in_angle);
	RkFloat const sin = Sin(in_angle);

	return Matrix<2, 2> {
		 cos, sin,
		-sin, cos
	};
}

template<RkSize TRows, RkSize TColumns>
template<RkSize TSize>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::ScaleMatrix(Vector<TSize, Meters> const& in_scale) noexcept
	requires (is_square && TRows >= TSize)
{
	Matrix scale {};

	for (RkSize i = 0; i < TSize; i++)
		scale[i, i] = static_cast<RkFloat>(in_scale.data[i]);

	return scale;
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::TranslationMatrix(Vector<TRows - 1, Meters> const& in_translation) noexcept
	requires (TRows == TColumns)
{
	Matrix translation {};

	for (RkSize dimension {}; dimension < TRows - 1; ++dimension)
		translation[TRows - 1, dimension] = static_cast<RkFloat>(in_translation.data[dimension]);

	return translation;
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::Inverted() const noexcept requires (TRows == 4 && TColumns == 4)
{
	float A2323 = (*this)[2, 2] * (*this)[3, 3] - (*this)[2, 3] * (*this)[3, 2];
	float A1323 = (*this)[2, 1] * (*this)[3, 3] - (*this)[2, 3] * (*this)[3, 1];
	float A1223 = (*this)[2, 1] * (*this)[3, 2] - (*this)[2, 2] * (*this)[3, 1];
	float A0323 = (*this)[2, 0] * (*this)[3, 3] - (*this)[2, 3] * (*this)[3, 0];
	float A0223 = (*this)[2, 0] * (*this)[3, 2] - (*this)[2, 2] * (*this)[3, 0];
	float A0123 = (*this)[2, 0] * (*this)[3, 1] - (*this)[2, 1] * (*this)[3, 0];
	float A2313 = (*this)[1, 2] * (*this)[3, 3] - (*this)[1, 3] * (*this)[3, 2];
	float A1313 = (*this)[1, 1] * (*this)[3, 3] - (*this)[1, 3] * (*this)[3, 1];
	float A1213 = (*this)[1, 1] * (*this)[3, 2] - (*this)[1, 2] * (*this)[3, 1];
	float A2312 = (*this)[1, 2] * (*this)[2, 3] - (*this)[1, 3] * (*this)[2, 2];
	float A1312 = (*this)[1, 1] * (*this)[2, 3] - (*this)[1, 3] * (*this)[2, 1];
	float A1212 = (*this)[1, 1] * (*this)[2, 2] - (*this)[1, 2] * (*this)[2, 1];
	float A0313 = (*this)[1, 0] * (*this)[3, 3] - (*this)[1, 3] * (*this)[3, 0];
	float A0213 = (*this)[1, 0] * (*this)[3, 2] - (*this)[1, 2] * (*this)[3, 0];
	float A0312 = (*this)[1, 0] * (*this)[2, 3] - (*this)[1, 3] * (*this)[2, 0];
	float A0212 = (*this)[1, 0] * (*this)[2, 2] - (*this)[1, 2] * (*this)[2, 0];
	float A0113 = (*this)[1, 0] * (*this)[3, 1] - (*this)[1, 1] * (*this)[3, 0];
	float A0112 = (*this)[1, 0] * (*this)[2, 1] - (*this)[1, 1] * (*this)[2, 0];

	float det = (*this)[0, 0] * ((*this)[1, 1] * A2323 - (*this)[1, 2] * A1323 + (*this)[1, 3] * A1223)
			  - (*this)[0, 1] * ((*this)[1, 0] * A2323 - (*this)[1, 2] * A0323 + (*this)[1, 3] * A0223)
			  + (*this)[0, 2] * ((*this)[1, 0] * A1323 - (*this)[1, 1] * A0323 + (*this)[1, 3] * A0123)
	          - (*this)[0, 3] * ((*this)[1, 0] * A1223 - (*this)[1, 1] * A0223 + (*this)[1, 2] * A0123);

	det = 1.0f / det;

	return Matrix {
	   det *   ((*this)[1, 1] * A2323 - (*this)[1, 2] * A1323 + (*this)[1, 3] * A1223),
	   det * - ((*this)[0, 1] * A2323 - (*this)[0, 2] * A1323 + (*this)[0, 3] * A1223),
	   det *   ((*this)[0, 1] * A2313 - (*this)[0, 2] * A1313 + (*this)[0, 3] * A1213),
	   det * - ((*this)[0, 1] * A2312 - (*this)[0, 2] * A1312 + (*this)[0, 3] * A1212),
	   det * - ((*this)[1, 0] * A2323 - (*this)[1, 2] * A0323 + (*this)[1, 3] * A0223),
	   det *   ((*this)[0, 0] * A2323 - (*this)[0, 2] * A0323 + (*this)[0, 3] * A0223),
	   det * - ((*this)[0, 0] * A2313 - (*this)[0, 2] * A0313 + (*this)[0, 3] * A0213),
	   det *   ((*this)[0, 0] * A2312 - (*this)[0, 2] * A0312 + (*this)[0, 3] * A0212),
	   det *   ((*this)[1, 0] * A1323 - (*this)[1, 1] * A0323 + (*this)[1, 3] * A0123),
	   det * - ((*this)[0, 0] * A1323 - (*this)[0, 1] * A0323 + (*this)[0, 3] * A0123),
	   det *   ((*this)[0, 0] * A1313 - (*this)[0, 1] * A0313 + (*this)[0, 3] * A0113),
	   det * - ((*this)[0, 0] * A1312 - (*this)[0, 1] * A0312 + (*this)[0, 3] * A0112),
	   det * - ((*this)[1, 0] * A1223 - (*this)[1, 1] * A0223 + (*this)[1, 2] * A0123),
	   det *   ((*this)[0, 0] * A1223 - (*this)[0, 1] * A0223 + (*this)[0, 2] * A0123),
	   det * - ((*this)[0, 0] * A1213 - (*this)[0, 1] * A0213 + (*this)[0, 2] * A0113),
	   det *   ((*this)[0, 0] * A1212 - (*this)[0, 1] * A0212 + (*this)[0, 2] * A0112),
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TColumns, TRows> Matrix<TRows, TColumns>::Transposed() const noexcept
{
	Matrix<TColumns, TRows> new_matrix;

	for (RkSize row {}; row < TRows;    ++row)
	for (RkSize col {}; col < TColumns; ++col)
		new_matrix[col, row] = (*this)[row, col];

	return new_matrix;
}

#pragma endregion

#pragma region Operators

template<RkSize TRows	  , RkSize TColumns>
template<RkSize TOtherRows, RkSize TOtherColumns>
	requires (TColumns == TOtherRows)
constexpr Matrix<TRows, TOtherColumns> Matrix<TRows, TColumns>::operator*
	(Matrix<TOtherRows, TOtherColumns> const& in_matrix) const noexcept
{
	Matrix<TRows, TOtherColumns> new_matrix;

	for (RkSize dst_row	   {}; dst_row	  < TRows;		   ++dst_row)
	for (RkSize dst_column {}; dst_column < TOtherColumns; ++dst_column)
	{
		new_matrix[dst_row, dst_column] = 0.0F;
		for (RkSize inner {}; inner < TColumns; inner++) {
			new_matrix[dst_row, dst_column] += (*this)[dst_row, inner] * in_matrix[inner, dst_column];
		}
	}

	return new_matrix;
}

template<RkSize TRows, RkSize TColumns>
template<RkSize TVectorSize, EDistanceUnit TDistanceUnit> requires (TColumns == TVectorSize)
constexpr Vector<TVectorSize, Distance<TDistanceUnit>> Matrix<TRows, TColumns>::operator*
	(Vector<TVectorSize, Distance<TDistanceUnit>> const& in_vector) const noexcept
{
	Matrix<1, TVectorSize> matrix {};
	for (RkSize size {}; size < TVectorSize; size++)
		matrix[0, size] = static_cast<RkFloat>(in_vector[size]);

	// Pre multiplication, row major vectors
	Matrix<1, TVectorSize>						 result		   {matrix * *this};
	Vector<TVectorSize, Distance<TDistanceUnit>> result_vector {};
	for (RkSize size {}; size < TVectorSize; size++)
		result_vector[size] = Distance<TDistanceUnit>(result[0, size]);

	return result_vector;
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns>& Matrix<TRows, TColumns>::operator*=(Matrix const& in_matrix) noexcept
{
	*this = *this * in_matrix;

	return *this;
}

template<RkSize TRows, RkSize TColumns>
constexpr RkBool Matrix<TRows, TColumns>::operator==(Matrix const& in_matrix) const noexcept
{
	for (RkSize index {}; index < elements; ++index)
		if (!RelativelyClose(data[index], in_matrix.data[index]))
			return false;

	return true;
}

template<RkSize TRows, RkSize TColumns>
constexpr RkBool Matrix<TRows, TColumns>::operator!=(Matrix const& in_matrix) const noexcept
{
	for (RkSize index {}; index < elements; ++index)
		if (!RelativelyClose(data[index], in_matrix.data[index]))
			return true;

	return false;
}

template<RkSize TRows, RkSize TColumns>
template<RkSize TNewRows, RkSize TNewColumns>
constexpr Matrix<TRows, TColumns>::operator Matrix<TNewRows, TNewColumns>() const noexcept
{
	// Will be identity if the matrix is a square matrix
	Matrix<TNewRows, TNewColumns> to_matrix {};

	for (RkSize row {0ULL}; row < std::min(TRows, TNewRows); ++row)
		memcpy( // Copy is done row by row,
			&to_matrix.data[row * TNewColumns],
			&          data[row * TColumns   ],
			sizeof(RkFloat) * std::min(TColumns, TNewColumns));

	return to_matrix;
}

template<RkSize TRows, RkSize TColumns>
constexpr RkFloat const& Matrix<TRows, TColumns>::operator[](RkSize const in_row, RkSize const in_column) const noexcept
{
	RUKEN_ASSERT(in_row < TRows && in_column < TColumns, "Matrix access might go out of bound");

	return data[in_column + in_row * TColumns];
}

template<RkSize TRows, RkSize TColumns>
constexpr RkFloat& Matrix<TRows, TColumns>::operator[](RkSize const in_row, RkSize const in_column) noexcept
{
	RUKEN_ASSERT(in_row < TRows && in_column < TColumns, "Matrix access might go out of bound");

	return data[in_column + in_row * TColumns];
}

#pragma endregion

END_RUKEN_NAMESPACE