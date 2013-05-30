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


int alphabeta(const GameState& gs, int alpha, int beta, int depthleft)
{
    if(depthleft == 0)
        return quiesce(gs, alpha, beta);

    assert(gs.get_stepsleft());

    std::vector<Delta> deltas;
    if (gs.get_stepsleft() >= 2) {
        generate_pushes(gs, &deltas);
        generate_pulls(gs, &deltas);
    }
    generate_steps(gs, &deltas);
    
    for (delta_it it=deltas.begin(); it != deltas.end(); ++it) {
        // XXX TODO: finish!!!
        // const int score = -alphaBeta((*it)->get_gamestate(), other_color(this_color),
                                      // -beta, -alpha, depthleft - 1);
        // if(score >= beta) {
            // return beta;   //  fail hard beta-cutoff
        // }
        // if(score > alpha) {
            // alpha = score; // alpha acts like max in MiniMax
        // }
    }
    return alpha;
}

/*
static void move_counter_recur_(const GameState& gs, const Color for_color, const uint8_t steps_left, 
                         hash_map *seen, uint64_t *nmoves)
{
    uint64_t hash;

    std::vector<Step> pushpull, singles;

    if (!steps_left)
        return;

    if (steps_left >= 2) {
        generate_pushes(gs, for_color, &pushpull);
        generate_pulls(gs, for_color, &pushpull);

        for(step_it it=pushpull.begin(); it != pushpull.end(); it += 2) {
            // make pushpull
            GameState new_gs = gs;
            take_step_and_capture(*it, &new_gs);
            take_step_and_capture(*(it+1), &new_gs);
            // get hash
            hash = new_gs.get_hash();
            // check for duplicates.
            if (seen->count(hash)) {
                if ((*seen)[hash] >= steps_left - 2)
                    continue;
            } else {
                (*nmoves)++;
            }
            (*seen)[hash] = steps_left - 2;
            move_counter_recur_(new_gs, for_color, steps_left - 2, seen, nmoves);
        }
    }
    
    generate_steps(gs, for_color, &singles);
    
    for(step_it it=singles.begin(); it != singles.end(); ++it) {
        GameState new_gs = gs;
        take_step_and_capture(*it, &new_gs);
        // get hash
        hash = new_gs.get_hash();
        // check for duplicates.
        if (seen->count(hash)) {
            if ((*seen)[hash] >= steps_left - 1)
                continue;
        } else {
            (*nmoves)++;
        }
        (*seen)[hash] = steps_left - 1;
        move_counter_recur_(new_gs, for_color, steps_left - 1, seen, nmoves);
    }
}

uint64_t move_counter(const GameState& gs, const Color for_color)
{
    hash_map seen(20000);
    seen.set_empty_key(ULLONG_MAX);
    uint64_t nmoves = 0;
    move_counter_recur_(gs, for_color, 4, &seen, &nmoves);
    return nmoves;
}
*/
