#include <gtest/gtest.h>

#include <cog2d/util/math/vector.hpp>

using namespace cog2d;

TEST(VectorTest, AvgTest)
{
	Vector a(2, 4);
	Vector b = a.avg(a);
	EXPECT_FLOAT_EQ(b.x, a.x);
	EXPECT_FLOAT_EQ(b.y, a.y);

	b = a.avg({4, 2});
	EXPECT_FLOAT_EQ(b.x, 3);
	EXPECT_FLOAT_EQ(b.y, 3);
}

TEST(VectorTest, MagnitudeTest)
{
	Vector v(1, 0);
	EXPECT_FLOAT_EQ(v.magnitude(), 1);
}
