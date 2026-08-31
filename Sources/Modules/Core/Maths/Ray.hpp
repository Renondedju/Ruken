#pragma once

#include "Core/Maths/Vector/DistanceVector.hpp"

BEGIN_RUKEN_NAMESPACE

struct Ray2
{
	Vector2m origin;
	Vector2m direction;
};

struct Ray3
{
	Vector3m origin;
	Vector3m direction;
};

END_RUKEN_NAMESPACE