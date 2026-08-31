#pragma once

#include "Core/Maths/Vector/DistanceVector.hpp"
#include "Core/Maths/Ray.hpp"
#include "Maths/Utility.hpp"

BEGIN_RUKEN_NAMESPACE
	struct Plane2
{
	Vector2m origin;
	Vector2m normal;

	std::optional<Vector2m> RayPlaneIntersection(Ray2 const& in_ray) const noexcept
	{
		RkFloat const denominator {normal.Dot(in_ray.direction)};
		RkFloat const t 		  {normal.Dot(origin - in_ray.origin) / denominator};

		if (Abs(denominator) > 0.0001f && t >= 0.0f)
			return in_ray.origin + in_ray.direction * t;

		return std::nullopt;
	}
};

struct Plane3
{
	Vector3m origin;
	Vector3m normal;

	std::optional<Vector3m> RayPlaneIntersection(Ray3 const& in_ray) const noexcept
	{
		RkFloat const denominator {normal.Dot(in_ray.direction)};
		RkFloat const t 		  {normal.Dot(origin - in_ray.origin) / denominator};

		if (Abs(denominator) > 0.0001f && t >= 0.0f)
			return in_ray.origin + in_ray.direction * t;

		return std::nullopt;
	}
};

END_RUKEN_NAMESPACE