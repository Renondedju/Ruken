#include <gtest/gtest.h>

#include <Core/Maths/Vector/DistanceVector.hpp>

USING_RUKEN_NAMESPACE

TEST(Vector, Equality)
{
	constexpr Vector2m a {1_m, 2_m};
	constexpr Vector2m b {2_m, 3_m};

	EXPECT_EQ(a, a);
	EXPECT_NE(a, b);
}