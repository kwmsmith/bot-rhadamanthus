#include "move.h"
#include "gtest/gtest.h"

#include <iostream>

#define ARRSIZE(a) (sizeof(a) / sizeof((a)[0]))

TEST(Move, generate_unique_moves) {
    std::string positions[] = {
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
            " +-----------------+\n",
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
    };

    unsigned int counts[] = {22, 15420};

    EXPECT_EQ(ARRSIZE(positions), ARRSIZE(counts));

    std::vector<Move> moves;
    GameState gs;

    for(unsigned int i=0; i<ARRSIZE(positions); ++i) {
        gs.clear();
        moves.clear();
        EXPECT_TRUE(gamestate_from_input(positions[i], &gs));
        generate_unique_moves(gs, &moves);
        EXPECT_EQ(moves.size(), counts[i]);
    }
}

TEST(Move, move_counter) {
    unsigned int counts[] = {2, 1, 15420};
    
    std::string positions[] = {
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
    };

    EXPECT_EQ(ARRSIZE(positions), ARRSIZE(counts));

    GameState gs;
    for(unsigned int i=0; i<ARRSIZE(positions); ++i) {
        gs.clear();
        EXPECT_TRUE(gamestate_from_input(positions[i], &gs));
        EXPECT_EQ(move_counter(gs), counts[i]);
    }
}


TEST(Move, generate_moves) {

    unsigned int counts[] = {3, 2, 13, 12, 13, 34, 12133, 16877, 23887, 2, 204785};

    std::string positions[] = {
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
            "   a b c d e f g h\n",
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
    };

    EXPECT_EQ(ARRSIZE(positions), ARRSIZE(counts));

    std::vector<Move> moves;
    GameState gs;

    for(unsigned int i=0; i<ARRSIZE(positions); ++i) {
        gs.clear();
        moves.clear();
        EXPECT_TRUE(gamestate_from_input(positions[i], &gs));
        generate_moves(gs, &moves);
        EXPECT_EQ(moves.size(), counts[i]);
    }
}

TEST(Move, clueless_benchmark) {

    // sum() of counts: 2987987
    unsigned int counts[] = {20229, 20203, 18569, 25293, 17386, 23695, 22409,
        31866, 17622, 10232, 14361, 79, 8596, 16477, 48665, 358, 19, 150, 3598,
        161, 2549, 141, 2030, 3045, 336488, 337916, 337922, 337922, 338030, 338034,
        338034, 315908};

    std::string positions[] = {
        "10w \n"
            " +-----------------+\n"
            "8| r r r     r r   |\n"
            "7| r   m c c h r r |\n"
            "6|   d         d   |\n"
            "5|         E       |\n"
            "4|                 |\n"
            "3|   H     e   D   |\n"
            "2| R   C     C   R |\n"
            "1| R R R D M R R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "21w \n"
            " +-----------------+\n"
            "8| r r r       r   |\n"
            "7| r   m h c r   r |\n"
            "6|   d   E     d   |\n"
            "5|   r             |\n"
            "4|                 |\n"
            "3|   H     e   D   |\n"
            "2| R   C   C     R |\n"
            "1| R R R   M R R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "22w \n"
            " +-----------------+\n"
            "8| r r r       r   |\n"
            "7| r   m h c r   r |\n"
            "6|   d   E     d   |\n"
            "5| r               |\n"
            "4|                 |\n"
            "3|   H     e   D   |\n"
            "2| R   C   C     R |\n"
            "1| R R R M   R R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "6b \n"
            " +-----------------+\n"
            "8| r r r m   r r r |\n"
            "7| r   c     d   r |\n"
            "6|   h     c   h   |\n"
            "5|       d E       |\n"
            "4|                 |\n"
            "3| H H     e   M   |\n"
            "2| D D R     C C   |\n"
            "1| R R R R   R R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "23b \n"
            " +-----------------+\n"
            "8| r r r   m r r r |\n"
            "7| r   d     c   r |\n"
            "6| D   d E     h   |\n"
            "5| h   H c         |\n"
            "4|       C     H   |\n"
            "3|   R   e     M D |\n"
            "2|     R     C   R |\n"
            "1| R R         R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "11w \n"
            " +-----------------+\n"
            "8| r r r     r r r |\n"
            "7|     c   r c     |\n"
            "6|   d         h   |\n"
            "5| h               |\n"
            "4|         E   d   |\n"
            "3|   C     e   H   |\n"
            "2| R   D D   C   R |\n"
            "1| R R R   M R R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "18b \n"
            " +-----------------+\n"
            "8| r r r   m r r r |\n"
            "7| r   c d c d   r |\n"
            "6|   h         h   |\n"
            "5|                 |\n"
            "4| R     E         |\n"
            "3|   H   e     H   |\n"
            "2| M   D C C D   R |\n"
            "1| R R R     R R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "25w \n"
            " +-----------------+\n"
            "8| r r   m r   r r |\n"
            "7| r c d c   d r   |\n"
            "6|   h   e E   h   |\n"
            "5|                 |\n"
            "4|               R |\n"
            "3|   H         H   |\n"
            "2|     D   D C C   |\n"
            "1| R R R   M   R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "40w \n"
            " +-----------------+\n"
            "8| r r     m r   r |\n"
            "7| r c d h   d r R |\n"
            "6|       e E   r   |\n"
            "5|                 |\n"
            "4|                 |\n"
            "3|         D   H   |\n"
            "2|       D   C C   |\n"
            "1| R R R M     R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "14b \n"
            " +-----------------+\n"
            "8| r r r m   r r r |\n"
            "7| r d d c     c r |\n"
            "6|   h           h |\n"
            "5|                 |\n"
            "4|                 |\n"
            "3|   H   e     H E |\n"
            "2| R D D C M C   R |\n"
            "1| R R R   R   R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "24b \n"
            " +-----------------+\n"
            "8| r r r   m r r r |\n"
            "7| r d d c     c r |\n"
            "6|   h         h   |\n"
            "5|                 |\n"
            "4|                 |\n"
            "3| C H   e     R H |\n"
            "2| R D D E   C R R |\n"
            "1| R R R   M     R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "12w \n"
            " +-----------------+\n"
            "8| r e             |\n"
            "7|                 |\n"
            "6|                 |\n"
            "5|                 |\n"
            "4|                 |\n"
            "3|                 |\n"
            "2|                 |\n"
            "1| R E             |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "48b \n"
            " +-----------------+\n"
            "8| r         D r   |\n"
            "7| r     r         |\n"
            "6|   c             |\n"
            "5|           c     |\n"
            "4| R r h           |\n"
            "3|     D r     E   |\n"
            "2|     C d R R d R |\n"
            "1|       R       R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "48b \n"
            " +-----------------+\n"
            "8| r         D r   |\n"
            "7| r     r         |\n"
            "6|   c             |\n"
            "5|           c     |\n"
            "4| R r h           |\n"
            "3|         r e E   |\n"
            "2|   D C d R R d R |\n"
            "1|         R     R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "20w \n"
            " +-----------------+\n"
            "8| r r   m h   r   |\n"
            "7| r   M     r   r |\n"
            "6| r     E         |\n"
            "5|   R           H |\n"
            "4|               e |\n"
            "3| D       C     R |\n"
            "2|     H C   R     |\n"
            "1| R R R       R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "16w \n"
            " +-----------------+\n"
            "8|     e           |\n"
            "7| R               |\n"
            "6|                 |\n"
            "5|           E     |\n"
            "4|                 |\n"
            "3|       R         |\n"
            "2|                 |\n"
            "1|                 |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "16b \n"
            " +-----------------+\n"
            "8|     e           |\n"
            "7|                 |\n"
            "6|                 |\n"
            "5|           E     |\n"
            "4|                 |\n"
            "3|       R         |\n"
            "2|                 |\n"
            "1|                 |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "16w \n"
            " +-----------------+\n"
            "8|     e           |\n"
            "7|                 |\n"
            "6|                 |\n"
            "5|           E     |\n"
            "4|                 |\n"
            "3|       R         |\n"
            "2|                 |\n"
            "1|                 |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "26b \n"
            " +-----------------+\n"
            "8| r r r r r   r r |\n"
            "7| M E d h         |\n"
            "6| h D C c c   R H |\n"
            "5| R R R R r m   e |\n"
            "4|             H R |\n"
            "3|                 |\n"
            "2|         R       |\n"
            "1|                 |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "46b \n"
            " +-----------------+\n"
            "8|                 |\n"
            "7|                 |\n"
            "6|                 |\n"
            "5|                 |\n"
            "4|         r   r   |\n"
            "3| r   r   E   D r |\n"
            "2| R M e R m     R |\n"
            "1|   h R           |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "61w \n"
            " +-----------------+\n"
            "8| r H r           |\n"
            "7|   m E           |\n"
            "6|   r R d c   r   |\n"
            "5|   R   R R   R r |\n"
            "4| R D e R   r C r |\n"
            "3|                 |\n"
            "2|                 |\n"
            "1|                 |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "58b \n"
            " +-----------------+\n"
            "8|                 |\n"
            "7|                 |\n"
            "6|                 |\n"
            "5|                 |\n"
            "4|   C         M r |\n"
            "3| R e r E         |\n"
            "2|     D r r r   R |\n"
            "1|     R R R R R   |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "40b \n"
            " +-----------------+\n"
            "8|         r   r r |\n"
            "7| r             c |\n"
            "6|         D       |\n"
            "5|   H   R         |\n"
            "4|           M     |\n"
            "3|   m           H |\n"
            "2|   E e R       r |\n"
            "1|             R R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8| r r     r   H   |\n"
            "7|     c r R h   r |\n"
            "6|   d       H R   |\n"
            "5|             e   |\n"
            "4|   h E       r C |\n"
            "3|   d R c     R   |\n"
            "2|     R           |\n"
            "1|                 |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   E   M   C     |\n"
            "6|                 |\n"
            "5|     D   H   D   |\n"
            "4|       H   C     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   E   H   C     |\n"
            "6|                 |\n"
            "5|     D   H   D   |\n"
            "4|       M   C     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   C   H   E     |\n"
            "6|                 |\n"
            "5|     D   H   D   |\n"
            "4|       M   C     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   C   H   E     |\n"
            "6|                 |\n"
            "5|     D   H   D   |\n"
            "4|       M   C     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   C   H   E     |\n"
            "6|                 |\n"
            "5|     D   M   D   |\n"
            "4|       H   C     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   C   H   E     |\n"
            "6|                 |\n"
            "5|     D   M   D   |\n"
            "4|       C   H     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7|   E   H   C     |\n"
            "6|                 |\n"
            "5|     D   M   D   |\n"
            "4|       C   H     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
        "63w \n"
            " +-----------------+\n"
            "8|               r |\n"
            "7| E     H   C     |\n"
            "6|                 |\n"
            "5|     D   M   D   |\n"
            "4|       C   H     |\n"
            "3|   R             |\n"
            "2|     R   R   R   |\n"
            "1|   R   R   R   R |\n"
            " +-----------------+\n"
            "   a b c d e f g h\n",
    };

    EXPECT_EQ(ARRSIZE(positions), ARRSIZE(counts));

    std::vector<Move> moves;
    GameState gs;

    for(unsigned int i=0; i<ARRSIZE(positions); ++i) {
        gs.clear();
        moves.clear();
        EXPECT_TRUE(gamestate_from_input(positions[i], &gs));
        generate_unique_moves(gs, &moves);
        EXPECT_EQ(moves.size(), counts[i]+1);
    }
}

