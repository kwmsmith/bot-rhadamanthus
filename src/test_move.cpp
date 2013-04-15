#include "move.h"
#include "gtest/gtest.h"

TEST(Move, generate_moves) {
    std::vector<MovePtr> moves;
    GameState gs;
    std::string ss;
    
    ss =  
        "........"
        "........"
        "..x..x.."
        ".rRr...."
        ".r.r...."
        "..x..x.."
        "........"
        "........"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 3);

    ss =  
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        "rRRr.x.."
        "r.r....."
        ".r......"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 2);

    ss = 
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        "..x..x.."
        "RRRRRR.."
        "r....R.."
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, B, &moves);
    EXPECT_EQ(moves.size(), 13);

    ss =  
        "r...RR.."
        "RRR.RR.."
        "..x..x.."
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, B, &moves);
    EXPECT_EQ(moves.size(), 12);

    ss =  
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        "..x..x.."
        "RRR.RR.."
        "r...RR.."
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, B, &moves);
    EXPECT_EQ(moves.size(), 13);
    
    ss =  
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        "..x..x.."
        "R......."
        "........"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 34);
    
    ss =  
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        "..x..x.."
        "RRRRRRRR"
        "........"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 12133);
    
    ss =  
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        "RRRRRRRR"
        "........"
        "........"
        ;
    moves.clear();
    gs.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 16877);

    ss =  
        "RRRRRRRR"
        "CCDDHHME"
        "..x..x.."
        "........"
        "........"
        "..x..x.."
        "........"
        "........"
        ;
    gs.clear();
    moves.clear();
    EXPECT_TRUE(gamestate_from_string(ss, &gs));
    generate_moves(gs, W, &moves);
    EXPECT_EQ(moves.size(), 23887);

    // ss = 
        // "..RRRRm."
        // "R.C...ER"
        // "..x.ex.r"
        // "DdRr...h"
        // ".....MHr"
        // "..x.Hxd."
        // "r.r.rr.."
        // ".rc..c.."
        // ;

    // ss = 
        // "..RRRRm."
        // "R.C...ER"
        // "..x.ex.r"
        // "DdRr...h"
        // ".....MHr"
        // "..x.Hxd."
        // "r.r.rr.."
        // ".rc..c.."
        // ;
    // gs.clear();
    // moves.clear();
    // EXPECT_TRUE(gamestate_from_string(ss, &gs));
    // generate_moves(gs, W, &moves);
    // EXPECT_EQ(moves.size(), 23887);
}
