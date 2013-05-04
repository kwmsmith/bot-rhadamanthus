#include "search.h"
#include "move.h"

static int quiesce(const GameState& gs, const Color& this_color, int alpha, int beta)
{
    // TODO: Implement quiescence search!!!
    return evaluate(gs, this_color);
}

/*
int quiesce(int alpha, int beta) {
    int stand_pat = Evaluate();
    if( stand_pat >= beta )
        return beta;
    if( alpha < stand_pat )
        alpha = stand_pat;
 
    until( every_capture_has_been_examined )  {
        MakeCapture();
        score = -Quiesce( -beta, -alpha );
        TakeBackMove();
 
        if( score >= beta )
            return beta;
        if( score > alpha )
           alpha = score;
    }
    return alpha;
}
*/


int alphabeta(const GameState& gs, const Color& this_color, int alpha, int beta, int depthleft)
{
    if(depthleft == 0) {
        return quiesce(gs, this_color, alpha, beta);
    }
    std::vector<MovePtr> moves;
    generate_unique_moves(gs, this_color, &moves);
    for (vec_move_it it=moves.begin(); it!=moves.end(); ++it) {
        const int score = -alphaBeta((*it)->get_gamestate(), other_color(this_color),
                                      -beta, -alpha, depthleft - 1);
        if(score >= beta) {
            return beta;   //  fail hard beta-cutoff
        }
        if(score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
        }
    }
    return alpha;
}
