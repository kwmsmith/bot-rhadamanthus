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

TEST(GameState, init_from_string) {
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
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
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

    gs.add_piece_at(B, R, 0);
    gs.add_piece_at(W, R, 1);
    gs.add_piece_at(W, E, 8);
    EXPECT_EQ(gs.frozen_pieces(W), 0);
    EXPECT_EQ(gs.frozen_pieces(B), 1);

    gs.clear();

    gs.add_piece_at(B, E, 19);
    gs.add_piece_at(W, M, 20);
    gs.add_piece_at(W, H, 18);
    gs.add_piece_at(W, H, 11);
    gs.add_piece_at(W, D, 27);
    EXPECT_EQ(gs.frozen_pieces(B), 0);
    Board w_frozen = gs.frozen_pieces(W);
    EXPECT_TRUE(w_frozen.contains(20));
    EXPECT_TRUE(w_frozen.contains(18));
    EXPECT_TRUE(w_frozen.contains(11));
    EXPECT_TRUE(w_frozen.contains(27));
    EXPECT_FALSE(w_frozen.contains(28));

    gs.clear();

    gs.add_piece_at(B, E, 19);
    gs.add_piece_at(W, E, 20);
    EXPECT_EQ(gs.frozen_pieces(B), 0);
    EXPECT_EQ(gs.frozen_pieces(W), 0);
}

TEST(GameState, MobilePieces) {
    // TODO: test edge cases.
    GameState gs;

    EXPECT_EQ(gs.mobile_pieces(B), 0);
    EXPECT_EQ(gs.mobile_pieces(W), 0);

    gs.add_piece_at(B, E, 'A', 1);
    EXPECT_TRUE(gs.mobile_pieces(B).contains(0));

    gs.add_piece_at(W, R, 'A', 2);
    gs.add_piece_at(W, R, 'B', 1);

    EXPECT_EQ(gs.mobile_pieces(B), 0);
    EXPECT_EQ(gs.mobile_pieces(W), 0);
}

TEST(GameState, move_piece) {
    // TODO: Test edge cases.
    GameState gs;

    gs.add_piece_at(B, E, 32);
    EXPECT_TRUE(gs.contains_at(B, E, 32));

    EXPECT_EQ(gs.move_piece(B, E, 32, 33), 1);

    EXPECT_FALSE(gs.contains_at(B, E, 32));
    EXPECT_TRUE(gs.contains_at(B, E, 33));
}
