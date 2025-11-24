#include <gtest/gtest.h>
#include <cmath>
#include "fpencoder.h"

TEST(FpEncoder, TestWithError)
{
	FPEncoder encoder = FPEncoder::WithPrecision(0.0, 1e5, 2);

	auto encoded = encoder.Encode(M_PI);
	EXPECT_EQ(encoded.size(), 4);

	auto decoded = encoder.Decode(encoded);

	EXPECT_NEAR(decoded, M_PI, 1e-4);
}
