#pragma once

#include "Core/Maths/Vector/Helper/VectorForward.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TDestination, typename TSource>
Vector<2, TDestination> VectorCast(Vector<2, TSource> const& in_vector) noexcept
{
	return Vector<2, TDestination>(
		static_cast<TDestination>(in_vector[0]),
		static_cast<TDestination>(in_vector[1])
	);
}

template <typename TDestination, typename TSource>
Vector<3, TDestination> VectorCast(Vector<3, TSource> const& in_vector) noexcept
{
	return Vector<3, TDestination>(
		static_cast<TDestination>(in_vector[0]),
		static_cast<TDestination>(in_vector[1]),
		static_cast<TDestination>(in_vector[2])
	);
}

template <typename TDestination, typename TSource>
Vector<4, TDestination> VectorCast(Vector<4, TSource> const& in_vector) noexcept
{
	return Vector<4, TDestination>(
		static_cast<TDestination>(in_vector[0]),
		static_cast<TDestination>(in_vector[1]),
		static_cast<TDestination>(in_vector[2]),
		static_cast<TDestination>(in_vector[3])
	);
}

END_RUKEN_NAMESPACE