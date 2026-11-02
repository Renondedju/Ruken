#pragma once

#include "Core/Maths/Vector/DistanceVector.hpp"
#include "Core/Maths/Ray.hpp"
#include "Maths/Utility.hpp"

BEGIN_RUKEN_NAMESPACE

template <RkSize TDimensions>
struct Plane
{
	Vector<TDimensions, Meters> origin;
	Vector<TDimensions, Meters> normal;

	std::optional<Meters> RayPlaneDistance(Ray<TDimensions> const& in_ray) const noexcept
	{
		RkFloat const denominator {normal.Dot(in_ray.direction)};
		RkFloat const t 		  {normal.Dot(origin - in_ray.origin) / denominator};

		if (Abs(denominator) > 0.001f && t >= 0.0f)
			return Meters(t);

		return std::nullopt;
	}

	std::optional<Vector<TDimensions, Meters>> RayPlaneIntersection(Ray<TDimensions> const& in_ray) const noexcept
	{
		if (auto intersection_distance {RayPlaneDistance(in_ray)})
			return in_ray.origin + in_ray.direction * intersection_distance.value();

		return std::nullopt;
	}

	static Vector<TDimensions, Meters> ProjectVectorToPlane(
		Vector<TDimensions, Meters> const& in_vector,
		Vector<TDimensions, Meters> const& in_plane_normal) noexcept
	{
		return in_vector - in_plane_normal * in_vector.Dot(in_plane_normal);
	}
};

using Plane2 = Plane<2>;
using Plane3 = Plane<3>;

END_RUKEN_NAMESPACE