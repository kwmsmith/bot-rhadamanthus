#include <inttypes.h>
#include <sparsehash/dense_hash_set>

#include "move.h"

typedef google::dense_hash_set<uint64_t, GameStateHash> hash_set;

void generate_unique_moves(const GameState& gs, const Color for_color, std::vector<Move> *moves)
{
    uint64_t hash;
    std::vector<Step> vec_step;
    hash_set seen(20000);
    seen.set_empty_key(ULLONG_MAX);
    moves->reserve(1000);
    size_t idx = moves->size();
    
    moves->push_back(Move(gs));
    seen.insert(gs.get_hash());

    while (idx < moves->size()) {
        Move this_move = (*moves)[idx];
        const GameState& gs = this_move.get_gamestate();
        const unsigned int steps_left = this_move.get_stepsleft();
        if (steps_left) {
            vec_step.clear();
            generate_steps(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
                Move mv = Move(this_move);
                mv.add_step(*it);
                hash = mv.get_zobrist_hash();
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
                Move mv = Move(this_move);
                mv.add_step(*it).add_step(*(it+1));
                hash = mv.get_zobrist_hash();
                if (!seen.count(hash)) {
                    seen.insert(hash);
                    moves->push_back(mv);
                }
            }
        }
        ++idx;
    }
}

void generate_moves(const GameState& gs, const Color for_color, std::vector<Move> *moves)
{
    std::vector<Step> vec_step;
    size_t idx = moves->size();
    moves->push_back(Move(gs));

    while (idx < moves->size()) {
        Move this_move = (*moves)[idx];
        const GameState& gs = this_move.get_gamestate();
        const unsigned int steps_left = this_move.get_stepsleft();
        if (steps_left) {
            vec_step.clear();
            generate_steps(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
                Move mv = Move(this_move);
                mv.add_step(*it);
                moves->push_back(mv);
            }
        }
        if (steps_left >= 2) {
            vec_step.clear();
            generate_pushes(gs, for_color, &vec_step);
            generate_pulls(gs, for_color, &vec_step);
            for(step_it it=vec_step.begin(); it != vec_step.end(); it += 2) {
                Move mv = Move(this_move);
                mv.add_step(*it).add_step(*(it+1));
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
