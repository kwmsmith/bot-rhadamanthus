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
    GameState gb;
}

TEST(GameState, piece_enum) {
    EXPECT_TRUE(R < C && C < D && D < H && H < M && M < E);
    EXPECT_EQ(E - R, 5);
}

TEST(GameState, frozen_pieces) {
    // Implement tests for frozen_pieces()
    GameState gs;
    EXPECT_EQ(gs.frozen_pieces(B), 0);
    EXPECT_EQ(gs.frozen_pieces(W), 0);

    gs.add_piece_at(R, B, 0);
    gs.add_piece_at(R, W, 1);
    gs.add_piece_at(E, W, 8);
    EXPECT_EQ(gs.frozen_pieces(W), 0);
    EXPECT_EQ(gs.frozen_pieces(B), 1);

    gs.clear();

    gs.add_piece_at(E, B, 19);
    gs.add_piece_at(M, W, 20);
    gs.add_piece_at(H, W, 18);
    gs.add_piece_at(H, W, 11);
    gs.add_piece_at(D, W, 27);
    EXPECT_EQ(gs.frozen_pieces(B), 0);
    Board w_frozen = gs.frozen_pieces(W);
    EXPECT_TRUE(w_frozen.contains(20));
    EXPECT_TRUE(w_frozen.contains(18));
    EXPECT_TRUE(w_frozen.contains(11));
    EXPECT_TRUE(w_frozen.contains(27));
    EXPECT_FALSE(w_frozen.contains(28));

    gs.clear();

    gs.add_piece_at(E, B, 19);
    gs.add_piece_at(E, W, 20);
    EXPECT_EQ(gs.frozen_pieces(B), 0);
    EXPECT_EQ(gs.frozen_pieces(W), 0);
}

TEST(GameState, init_from_string) {
    GameState gb;
    EXPECT_TRUE(gb.init_from_string("----------------------------------------------------------------"));
    EXPECT_TRUE(gb.is_empty());
    EXPECT_TRUE(gb.init_from_string("R--------------------------------------------------------------R"));
    EXPECT_FALSE(gb.is_empty());
}

TEST(GameState, pieces_stronger) {
    GameState gs;
    gs.init_from_string("R--------------------------------------------------------------R");
}

TEST(GameState, MobilePieces) {
    // TODO: test edge cases.
    GameState gs;

    EXPECT_EQ(gs.mobile_pieces(B), 0);
    EXPECT_EQ(gs.mobile_pieces(W), 0);

    gs.add_piece_at(E, B, 'A', 1);
    EXPECT_TRUE(gs.mobile_pieces(B).contains(0));

    gs.add_piece_at(R, W, 'A', 2);
    gs.add_piece_at(R, W, 'B', 1);

    EXPECT_EQ(gs.mobile_pieces(B), 0);
    EXPECT_EQ(gs.mobile_pieces(W), 0);
}

TEST(GameState, move_piece) {
    // TODO: Test edge cases.
    GameState gs;

    gs.add_piece_at(E, B, 32);
    EXPECT_TRUE(gs.contains_at(E, B, 32));

    EXPECT_EQ(gs.move_piece(E, B, 32, 33), 1);

    EXPECT_FALSE(gs.contains_at(E, B, 32));
    EXPECT_TRUE(gs.contains_at(E, B, 33));
}

#include <cstdio>

TEST(GameState, make_step) {
    EXPECT_TRUE(make_step("Rb1n").is_move());
    EXPECT_TRUE(make_step("rb1n").is_move());
    EXPECT_TRUE(make_step("eb8").is_placement());
    EXPECT_TRUE(make_step("dc3x").is_capture());
    EXPECT_FALSE(make_step("qc5x").is_valid());
}
