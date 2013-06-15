#include "evaluate.h"
#include "gtest/gtest.h"

#include <cmath>

TEST(Evaluate, approx_log) {
    EXPECT_TRUE(fabs(log(1) - approx_log(1)) < 1e-5);
    EXPECT_TRUE(fabs(log(10) - approx_log(10)) < 1e-5);
    EXPECT_TRUE(fabs(log(20) - approx_log(20)) < 1e-5);
    EXPECT_TRUE(fabs(log(64) - approx_log(64)) < 1e-5);
}

TEST(Evaluate, eval_material) {
    const float normal = 0.1250700;
    float evw, evb;
    GameState gs;
    std::string ss;
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
        "   a b c d e f g h\n";
    gamestate_from_input(ss, &gs);
    evw = eval_material(gs);
    gs.flip_color();
    evb = eval_material(gs);
    EXPECT_EQ(evw, -evb);
    
    ss = 
        "10w\n"
        " +-----------------+\n"
        "8| r r r r r r r r |\n"
        "7| c c d d h h m e |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| C C D D H H M E |\n"
        "1| R R R R R R R R |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n" ;
    gs.clear();
    gamestate_from_input(ss, &gs);
    evw = eval_material(gs);
    gs.flip_color();
    evb = eval_material(gs);
    EXPECT_EQ(evw, -evb);
    EXPECT_EQ(evw, 0.0);
    
    ss = 
        "10w\n"
        " +-----------------+\n"
        "8| r r r r r r r r |\n"
        "7| c c d d h h m e |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| C C D D H H M E |\n"
        "1| . . . . . . . R |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n" ;
    gs.clear();
    gamestate_from_input(ss, &gs);
    evw = eval_material(gs);
    gs.flip_color();
    evb = eval_material(gs);
    EXPECT_EQ(evw, -evb);
    std::cout << evw << " " << evb << std::endl;
    EXPECT_TRUE(fabs(evw / normal + 13.40) < 0.01);
    
    ss = 
        "10w\n"
        " +-----------------+\n"
        "8| r r r r r r r r |\n"
        "7| c c d d h h m e |\n"
        "6| . . x . . x . . |\n"
        "5| . . . . . . . . |\n"
        "4| . . . . . . . . |\n"
        "3| . . x . . x . . |\n"
        "2| C C D D H H M E |\n"
        "1| . R R R R R R R |\n"
        " +-----------------+\n"
        "   a b c d e f g h\n" ;
    gs.clear();
    gamestate_from_input(ss, &gs);
    evw = eval_material(gs);
    gs.flip_color();
    evb = eval_material(gs);
    EXPECT_EQ(evw, -evb);
    EXPECT_TRUE(fabs(evw / normal + 1.00) < 0.01);
}
