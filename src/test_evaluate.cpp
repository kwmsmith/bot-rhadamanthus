#include "evaluate.h"
#include "gtest/gtest.h"

#include <cmath>

TEST(Evaluate, approx_log) {
    EXPECT_TRUE(fabs(log(1) - approx_log(1)) < 1e-5);
    EXPECT_TRUE(fabs(log(10) - approx_log(10)) < 1e-5);
    EXPECT_TRUE(fabs(log(20) - approx_log(20)) < 1e-5);
    EXPECT_TRUE(fabs(log(64) - approx_log(64)) < 1e-5);
}
