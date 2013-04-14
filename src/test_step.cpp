#include "gtest/gtest.h"
#include "step.h"
#include <cstdio>

TEST(GameState, make_step) {
    EXPECT_TRUE(make_step("Rb1n").is_motion());
    EXPECT_TRUE(make_step("rb1n").is_motion());
    EXPECT_TRUE(make_step("eb8").is_placement());
    EXPECT_TRUE(make_step("dc3x").is_capture());
    EXPECT_FALSE(make_step("qc5x").is_valid());
}
