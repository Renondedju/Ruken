#pragma once

#include "Core/Maths/Vector/DistanceVector.hpp"

BEGIN_RUKEN_NAMESPACE

template <RkSize TDimensions>
struct Ray
{
	Vector<TDimensions, Meters> origin;
	Vector<TDimensions, Meters> direction;
};

using Ray2 = Ray<2>;
using Ray3 = Ray<3>;

END_RUKEN_NAMESPACE