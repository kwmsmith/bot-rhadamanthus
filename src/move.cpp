#include "move.h"

void generate_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves)
{
    std::vector<MovePtr> unprocessed;
    unprocessed.push_back(MovePtr(new Move(gs)));
    
    std::vector<Step> vec_step;
    typedef std::vector<Step>::iterator vec_step_it;

    while (unprocessed.size()) {
        MovePtr mp = unprocessed.back();
        unprocessed.pop_back();
        moves->push_back(mp);
        const GameState& gs = mp->get_gamestate();
        const unsigned int steps_left = mp->get_stepsleft();
        if (steps_left >= 2) {
            vec_step.clear();
            generate_pushes(gs, for_color, &vec_step);
            generate_pulls(gs, for_color, &vec_step);
            for(vec_step_it it=vec_step.begin(); it != vec_step.end(); it += 2) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it).add_step(*(it+1));
                // TODO: Test for transpositions...
                unprocessed.push_back(mv);
            }
        } 
        if (steps_left >= 1) {
            vec_step.clear();
            generate_steps(gs, for_color, &vec_step);
            for(vec_step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it);
                // TODO: Test for transpositions...
                unprocessed.push_back(mv);
            }
        }
    }
}
