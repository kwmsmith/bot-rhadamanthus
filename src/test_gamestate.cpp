#include "gamestate.h"
#include "gtest/gtest.h"

TEST(Helpers, internal_idx_from_display) {
    EXPECT_EQ(internal_idx_from_display(0), 56);
    EXPECT_EQ(internal_idx_from_display(1), 57);
    EXPECT_EQ(internal_idx_from_display(7), 63);
    EXPECT_EQ(internal_idx_from_display(56), 0);
    EXPECT_EQ(internal_idx_from_display(63), 7);
}

TEST(Helpers, piece_from_char) {
    EXPECT_EQ(piece_from_char('H'), H);
    EXPECT_EQ(piece_from_char('h'), H);

    EXPECT_EQ(piece_from_char('M'), M);
    EXPECT_EQ(piece_from_char('m'), M);

    EXPECT_EQ(piece_from_char('q'), kInvalidPiece);
}

TEST(Helpers, color_from_char) {
    EXPECT_EQ(color_from_char('M'), W);
    EXPECT_EQ(color_from_char('D'), W);
    EXPECT_EQ(color_from_char('H'), W);
    EXPECT_EQ(color_from_char('R'), W);

    EXPECT_EQ(color_from_char('m'), B);
    EXPECT_EQ(color_from_char('d'), B);
    EXPECT_EQ(color_from_char('h'), B);
    EXPECT_EQ(color_from_char('r'), B);

    EXPECT_EQ(color_from_char('q'), kInvalidPiece);
}

TEST(GameState, Instantiation) {
    GameState gb = GameState();
}

TEST(GameState, init_from_string) {
    GameState gb = GameState();
    EXPECT_TRUE(gb.init_from_string("----------------------------------------------------------------"));
    EXPECT_TRUE(gb.is_empty());
    EXPECT_TRUE(gb.init_from_string("R--------------------------------------------------------------R"));
    EXPECT_FALSE(gb.is_empty());
}

TEST(GameState, pieces_stronger) {
    GameState gs = GameState();
    gs.init_from_string("R--------------------------------------------------------------R");
}

TEST(GameState, MobilePieces) {
    GameState gb2 = GameState();
    Board b = gb2.mobile_pieces(W);
    EXPECT_EQ(b, 0ULL);
}
