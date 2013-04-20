#include "move.h"

#include <iostream>
#include <inttypes.h>

typedef std::vector<Step>::iterator vec_step_it;

static void _generate_unique_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves)
{
    typedef std::vector<MovePtr>::const_iterator move_it;
    std::vector<MovePtr> all_moves;
    generate_moves(gs, for_color, &all_moves);

    hash_set seen;
    seen.set_empty_key(ULLONG_MAX);

    uint64_t hash;

    for (move_it it=all_moves.begin(); it != all_moves.end(); ++it) {
        hash = (*it)->get_zobrist_hash();
        if (seen.count(hash))
            continue;
        seen.insert(hash);
        moves->push_back(*it);
    }
}

void generate_unique_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves)
{
    uint64_t hash;
    std::vector<Step> vec_step;
    hash_set seen;
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
            for(vec_step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
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
            for(vec_step_it it=vec_step.begin(); it != vec_step.end(); it += 2) {
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
            for(vec_step_it it=vec_step.begin(); it != vec_step.end(); ++it) {
                MovePtr mv = MovePtr(new Move(*mp));
                mv->add_step(*it);
                moves->push_back(mv);
            }
        }
        if (steps_left >= 2) {
            vec_step.clear();
            generate_pushes(gs, for_color, &vec_step);
            generate_pulls(gs, for_color, &vec_step);
            for(vec_step_it it=vec_step.begin(); it != vec_step.end(); it += 2) {
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
    for (step_it it=steps_.begin(); it != steps_.end(); ++it) {
        s += it->to_std_string() + " ";
    }
    return s;
}
