#include <gtest/gtest.h>

#include <Core/Maths/Utility.hpp>

USING_RUKEN_NAMESPACE

TEST(Maths, RelativelyClose)
{
	constexpr RkFloat a {1.0f};
	constexpr RkFloat b {1.1f};
	constexpr RkFloat c {a + std::numeric_limits<RkFloat>::epsilon()};

	EXPECT_TRUE (RelativelyClose(a, a));
	EXPECT_TRUE (RelativelyClose(a, c));
	EXPECT_FALSE(RelativelyClose(a, b));
	EXPECT_FALSE(RelativelyClose(c, b));
}