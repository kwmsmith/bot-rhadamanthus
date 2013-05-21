#include "move.h"
#include "gtest/gtest.h"

#include <iostream>

TEST(Move, generate_unique_moves) {
    std::vector<Move> moves;
    GameState gs;
    Color cc;
    std::string ss;

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| . . . . . . . . |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . R . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        ;
    gs.clear();
    moves.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_unique_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 22);

    ss = 
        "10w\n"
        " +-----------------+\n"
        "8| . r c . . c . . |\n"
        "7| r . r . r r . . |\n"
        "6| . . x . H x d . |\n"
        "5| . . . . . M H r |\n"
        "4| D d R r . . . h |\n"
        "3| . . x . e x . r |\n"
        "2| R . C . . . E R |\n"
        "1| . . R R R R m . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    gs.clear();
    moves.clear();
    gamestate_from_input(ss, &gs, &cc);
    generate_unique_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 15420);
}


TEST(Move, generate_moves) {
    std::vector<Move> moves;
    GameState gs;
    Color cc;
    std::string ss;

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| . . . . . . . . |\n"
        "6| . . x . . x . . |\n"
        "5| . r . r . . . . |\n"
        "4| . r R r . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 3);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . r . . . . . . |\n"
        "7| r . r . . . . . |\n"
        "6| r R R r . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 2);

    ss =
        "10b\n"
        " +-----------------+\n"
        "8| r . . . . R . . |\n"
        "7| R R R R R R . . |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, B, &moves);
    EXPECT_EQ(moves.size(), 13);

    ss =
        "10b\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| . . . . . . . . |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| R R R . R R . . |\n"
        "1| r . . . R R . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, B, &moves);
    EXPECT_EQ(moves.size(), 12);

    ss =
        "10b\n"
        " +-----------------+\n"
        "8| r . . . R R . . |\n"
        "7| R R R . R R . . |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, B, &moves);
    EXPECT_EQ(moves.size(), 13);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| R . . . . . . . |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 34);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| R R R R R R R R |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 12133);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| . . . . . . . . |\n"
        "6| R R R R R R R R |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 16877);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . . . |\n"
        "7| . . . . . . . . |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| C C D D H H M E |\n"
        "1| R R R R R R R R |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    gs.clear();
    moves.clear();
    EXPECT_TRUE(gamestate_from_input(ss, &gs, &cc));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 23887);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . . . . . . c D |\n"
        "7| . . . . . . m d |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| . . . . . . . . |\n"
        "1| . . . . . . . . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    gs.clear();
    moves.clear();
    gamestate_from_input(ss, &gs, &cc);
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 2);

    ss =
        "10w\n"
        " +-----------------+\n"
        "8| . r c . . c . . |\n"
        "7| r . r . r r . . |\n"
        "6| . . x . H x d . |\n"
        "5| . . . . . M H r |\n"
        "4| D d R r . . . h |\n"
        "3| . . x . e x . r |\n"
        "2| R . C . . . E R |\n"
        "1| . . R R R R m . |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n"
        ;
    gs.clear();
    moves.clear();
    gamestate_from_input(ss, &gs, &cc);
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 204785);
}
