#pragma once

#include "Core/Maths/Matrix/Matrix.hpp"

#include <cstring>

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
constexpr Matrix<TRows, TColumns>::Matrix(TTypes... in_values) noexcept
{
	RkFloat values[] { static_cast<RkFloat>(in_values)... };

	for (RkSize column {0ULL}; column < TColumns; ++column)
		for (RkSize row {0ULL}; row < TRows; ++row)
			data[column + row * TColumns] = values[column + row * TColumns];
}

#pragma region Static Methods

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::LookAtMatrix(
	Vector3m const& in_from,
	Vector3m const& in_to,
	Vector3m const& in_up) noexcept requires ((TRows == 3 || TRows == 4) && TColumns == 4)
{
	Vector3m const forward{(in_to - in_from   ).Normalized()};
	Vector3m const right  {forward.Cross(in_up).Normalized()};
	Vector3m const up     {right  .Cross(forward)};

	return Matrix<3, 4> {
		 right.x(),           up.x(),         -forward.x(),
		 right.y(),           up.y(),         -forward.y(),
		 right.z(),           up.z(),         -forward.z(),
		-right.Dot(in_from), -up.Dot(in_from), forward.Dot(in_from)
	};
/*
	return Matrix<3, 4> {
		right   .x(),  right  .y(),  right  .z(), -right  .Dot(in_from),
		up      .x(),  up     .y(),  up     .z(), -up     .Dot(in_from),
		-forward.x(), -forward.y(), -forward.z(),  forward.Dot(in_from)
	};
	*/
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::ModelMatrix(
	Vector3m   const& in_position,
	Quaternion const& in_rotation,
	Vector3m   const& in_scale) noexcept requires (TRows == 4 && TColumns == 4)
{
	return Matrix::ScaleMatrix		(in_scale)    *
	       Matrix::RotationMatrix3D (in_rotation) *
	       Matrix::TranslationMatrix(in_position);
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::PerspectiveProjectionMatrix(
	Radians const in_fov,
	RkFloat const in_aspect,
	Meters  const in_near,
	Meters  const in_far) noexcept requires (TRows == 4 && TColumns == 4)
{
	// https://www.kdab.com/projection-matrices-with-vulkan-part-1/
	// https://www.kdab.com/projection-matrices-with-vulkan-part-2/
	// Post view correction is used. This matrix is specific to vulkan.
	RkFloat const fov_tangent = Tan(in_fov / 2.0F);

	Meters const far  {-in_far};
	Meters const near {-in_near};

	return Matrix<4, 4> {
		in_aspect / fov_tangent, 0.0F,  0.0F, 0.0F,
		0.0F, 1.0f / fov_tangent, 0.0F, 0.0F,
		0.0F, 0.0F, -far / (far - near),          1.0F,
		0.0F, 0.0F, -(far * near) / (far - near), 0.0F
	} * Matrix4x4::ClipSpace();
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::OrthogonalProjectionMatrix(
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
	} * Matrix4x4::ClipSpace();
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
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrix3D(
	Radians const in_angle_x,
	Radians const in_angle_y,
	Radians const in_angle_z) noexcept requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const cos_x = Cos(in_angle_x);
	RkFloat const sin_x = Sin(in_angle_x);
	RkFloat const cos_y = Cos(in_angle_y);
	RkFloat const sin_y = Sin(in_angle_y);
	RkFloat const cos_z = Cos(in_angle_z);
	RkFloat const sin_z = Sin(in_angle_z);

	// The resulting matrix will implicitly be converted to the requested size if needed
	return Matrix<3, 3> {
		cos_z * cos_y - sin_z * sin_x * sin_y,
		-sin_z * cos_x,
		cos_z * sin_y + sin_z * sin_x * cos_y,

		sin_z * cos_y + cos_z * sin_x * sin_y,
		cos_z * cos_x,
		sin_z * sin_y - cos_z * sin_x * cos_y,

		-cos_x * sin_y,
		sin_x,
		cos_x * cos_y
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrix3D(
	Vector3m const& in_axis,
	Radians  const in_angle) noexcept requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const cos_angle = Cos(in_angle);
	RkFloat const sin_angle = Sin(in_angle);

	// The matrix will implicitly be converted to the requested size if needed
	return Matrix<3, 3> {
		cos_angle + static_cast<RkFloat>(in_axis.x() * in_axis.x()) * (1 - cos_angle),
		in_axis.x() * in_axis.y() * (1 - cos_angle) - in_axis.z() * sin_angle,
		in_axis.x() * in_axis.z() * (1 - cos_angle) + in_axis.y() * sin_angle,
		in_axis.y() * in_axis.x() * (1 - cos_angle) + in_axis.z() * sin_angle,
		cos_angle + static_cast<RkFloat>(in_axis.y() * in_axis.y()) * (1 - cos_angle),
		in_axis.y() * in_axis.z() * (1 - cos_angle) - in_axis.x() * sin_angle,
		in_axis.z() * in_axis.x() * (1 - cos_angle) - in_axis.y() * sin_angle,
		in_axis.z() * in_axis.y() * (1 - cos_angle) + in_axis.x() * sin_angle,
		cos_angle + static_cast<RkFloat>(in_axis.z() * in_axis.z()) * (1 - cos_angle),
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrix3D(Quaternion const in_value) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const sqr_x (in_value.x * in_value.x);
	RkFloat const sqr_y (in_value.y * in_value.y);
	RkFloat const sqr_z (in_value.z * in_value.z);

	// The matrix will implicitly be converted to the requested size if needed
	return Matrix<3, 3> {
		1 - 2 * (sqr_y + sqr_z),
		2 * (in_value.x * in_value.y - in_value.z * in_value.w),
		2 * (in_value.x * in_value.z + in_value.y * in_value.w),

		2 * (in_value.x * in_value.y + in_value.z * in_value.w),
		1 - 2 * (sqr_x + sqr_z),
		2 * (in_value.y * in_value.z - in_value.x * in_value.w),

		2 * (in_value.x * in_value.z - in_value.y * in_value.w),
		2 * (in_value.y * in_value.z + in_value.x * in_value.w),
		1 - 2 * (sqr_x + sqr_y)
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrixX(Radians const in_angle) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const cos = Cos(in_angle);
	RkFloat const sin = Sin(in_angle);

	return Matrix<3, 3> {
		1.0F,  0.0F,  0.0F,
		0.0F,  cos,   -sin,
		0.0F,  sin,    cos
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrixY(Radians const in_angle) noexcept
	requires (TRows >= 3 && TColumns >= 3)
{
	RkFloat const cos = Cos(in_angle);
	RkFloat const sin = Sin(in_angle);

	return Matrix<3, 3> {
		cos,  0.0F,   sin,
		0.0F,  1.0F,  0.0F,
		-sin,  0.0F,   cos
	};
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::RotationMatrixZ(Radians const in_angle) noexcept
	requires (TRows >= 2 && TColumns >= 2)
{
	RkFloat const cos = Cos(in_angle);
	RkFloat const sin = Sin(in_angle);

	return Matrix<2, 2> {
		cos,  -sin,
		sin,   cos
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
constexpr Matrix<TRows, TColumns> Matrix<TRows, TColumns>::TranslationMatrix(Vector3m const& in_translation) noexcept
	requires (TRows == 4 && TColumns == 4)
{
	return Matrix<4, 4> {
		1.0F, 0.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0F, 0.0F,
		in_translation.x(), in_translation.y(), in_translation.z(), 1.0F
	};
}

#pragma endregion

#pragma region Operators

template<RkSize TRows, RkSize TColumns>
template<RkSize TOtherRows, RkSize TOtherColumns> requires (TColumns == TOtherRows)
constexpr Matrix<TRows, TOtherColumns> Matrix<TRows, TColumns>::operator*
	(Matrix<TOtherRows, TOtherColumns> const& in_matrix) const noexcept
{
	Matrix<TRows, TOtherColumns> new_matrix;

	for (RkSize row    {0ULL}; row    < TRows;         ++row)
	for (RkSize column {0ULL}; column < TOtherColumns; ++column)
	{
		new_matrix[row, column] = 0.0F;
		for (RkSize other_row {0ULL}; other_row < TOtherRows; ++other_row)
			new_matrix[row, column] += (*this)[row, other_row] * in_matrix[other_row, column];
	}

	return new_matrix;
}

template<RkSize TRows, RkSize TColumns>
constexpr Matrix<TRows, TColumns>& Matrix<TRows, TColumns>::operator*=(Matrix const& in_matrix) noexcept
{
	*this = *this * in_matrix;

	return *this;
}

template<RkSize TRows, RkSize TColumns>
template<RkSize TNewRows, RkSize TNewColumns>
constexpr Matrix<TRows, TColumns>::operator Matrix<TNewRows, TNewColumns>() const noexcept
{
	// Will be identity if the matrix is a square matrix
	Matrix<TNewRows, TNewColumns> to_matrix {};

	for (RkSize row {0ULL}; row < std::min(TRows, TNewRows); ++row)
		std::memcpy( // Copy is done row by row,
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