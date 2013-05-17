#include <inttypes.h>
#include <sparsehash/dense_hash_set>

#include "move.h"

typedef google::dense_hash_set<uint64_t, GameStateHash> hash_set;

void generate_unique_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves)
{
    uint64_t hash;
    std::vector<Step> vec_step;
    hash_set seen(20000);
    seen.set_empty_key(ULLONG_MAX);
    size_t idx = moves->size();
    
    moves->push_back(MovePtr(new Move(gs)));
    seen.insert(gs.get_hash());

    while (idx < moves->size()) {
        MovePtr mp = moves->at(idx);
        const GameState& gs = mp->get_gamestate();
        const unsigned int steps_left = mp->get_stepsleft();
        if (steps_left >= 1) {
            vec_step.clear();
            generate_steps(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it);
                hash = mv->get_zobrist_hash();
                if (!seen.count(hash)) {
                    seen.insert(hash);
                    moves->push_back(mv);
                }
            }
        }
        if (steps_left >= 2) {
            vec_step.clear();
            generate_pushes(gs, for_color, &vec_step);
            generate_pulls(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); it += 2) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it).add_step(*(it+1));
                hash = mv->get_zobrist_hash();
                if (!seen.count(hash)) {
                    seen.insert(hash);
                    moves->push_back(mv);
                }
            }
        }
        ++idx;
    }
}

void generate_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves)
{
    std::vector<Step> vec_step;
    size_t idx = moves->size();
    moves->push_back(MovePtr(new Move(gs)));

    while (idx < moves->size()) {
        MovePtr mp = moves->at(idx);
        const GameState& gs = mp->get_gamestate();
        const unsigned int steps_left = mp->get_stepsleft();
        if (steps_left >= 1) {
            vec_step.clear();
            generate_steps(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it);
                moves->push_back(mv);
            }
        }
        if (steps_left >= 2) {
            vec_step.clear();
            generate_pushes(gs, for_color, &vec_step);
            generate_pulls(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); it += 2) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it).add_step(*(it+1));
                moves->push_back(mv);
            }
        } 
        ++idx;
    }
}

const std::string Move::to_std_string() const
{
    std::string s;
    for (int i=0; i < total_steps_taken_; ++i) {
        s += steps_[i].to_std_string() + " ";
    }
    return s;
}
