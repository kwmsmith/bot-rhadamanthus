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
    GameState gs;
    std::string ss;
    ss = 
        "..RRRRm."
        "R.C...ER"
        "..x.ex.r"
        "DdRr...h"
        ".....MHr"
        "..x.Hxd."
        "r.r.rr.."
        ".rc..c.."
        ;
    gamestate_from_string(ss, &gs);
    EXPECT_EQ(eval_material(gs, W), -eval_material(gs, B));
    
    ss = 
        "RRRRRRRR"
        "CCDDHHME"
        "........"
        "........"
        "........"
        "........"
        "ccddhhme"
        "rrrrrrrr"
        ;
    gs.clear();
    gamestate_from_string(ss, &gs);
    EXPECT_EQ(eval_material(gs, W), -eval_material(gs, B));
    EXPECT_EQ(eval_material(gs, W), 0.0);
    
    ss = 
        ".......R"
        "CCDDHHME"
        "........"
        "........"
        "........"
        "........"
        "ccddhhme"
        "rrrrrrrr"
        ;
    gs.clear();
    gamestate_from_string(ss, &gs);
    EXPECT_EQ(eval_material(gs, W), -eval_material(gs, B));
    EXPECT_TRUE(fabs(eval_material(gs, W) / normal + 13.40) < 0.01);
    
    ss = 
        ".RRRRRRR"
        "CCDDHHME"
        "........"
        "........"
        "........"
        "........"
        "ccddhhme"
        "rrrrrrrr"
        ;
    gs.clear();
    gamestate_from_string(ss, &gs);
    EXPECT_EQ(eval_material(gs, W), -eval_material(gs, B));
    EXPECT_TRUE(fabs(eval_material(gs, W) / normal + 1.00) < 0.01);
}
