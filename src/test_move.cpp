#include "move.h"
#include "gtest/gtest.h"

#include <iostream>

TEST(Move, generate_moves) {
    std::vector<MovePtr> moves;
    GameState gs;
    std::string ss(
            "RRRRRRRR"
            "CCDDHHME"
            "..x..x.."
            "........"
            "........"
            "..x..x.."
            "ccddhhme"
            "rrrrrrrr");
    // std::string ss(
            // "........"
            // "...E...."
            // "..x..x.."
            // "........"
            // "........"
            // "..x..x.."
            // "...e...."
            // "........");
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_TRUE(moves.size() > 0);
}
