#include "move.h"
#include <iostream>

const int NN = 20;

typedef std::vector<MovePtr>::const_iterator move_it;

int main(int argc, char **argv)
{
    std::vector<MovePtr> moves;
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
    gs.clear();
    for(int i=0; i < NN; ++i) {
        moves.clear();
        gamestate_from_string(ss, &gs);
        generate_moves(gs, W, &moves);
    }
    // EXPECT_EQ(moves.size(), 204785);

    return 0;
}
