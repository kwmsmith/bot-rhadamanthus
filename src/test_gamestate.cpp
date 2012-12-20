#include "gamestate.h"
#include "gtest/gtest.h"

TEST(GameState, Instantiation) {
    GameState gb = GameState();
}

TEST(GameState, MobilePieces) {
    GameState gb2 = GameState();
    Board b = gb2.mobile_pieces(W);
    EXPECT_EQ(b._board, 0ULL);
}
