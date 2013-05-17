#ifndef _MOVE_H_
#define _MOVE_H_

#include <boost/shared_ptr.hpp>

#include "step.h"
#include "gamestate.h"

class Move;

size_t move_hash(const Move& m);

struct GameStateHash
{
    size_t operator()(const uint64_t& key) const {
        return key;
    }
};

typedef std::vector<Step>::const_iterator step_it;

class Move
{
    public:

        Move(const GameState& gs) : total_steps_taken_(0) {
            gs.copy_to(&gs_); 
        }
        
        Move(const Move& m) {
            for(int i=0; i<MAXSTEPS; ++i)
                steps_[i] = m.steps_[i];
            m.gs_.copy_to(&gs_);
            total_steps_taken_ = m.total_steps_taken_;
        }
        
        Move& add_step(const Step& step) {
            assert(step.is_motion());
            assert(get_stepsleft() >= 1);
            steps_[total_steps_taken_] = step;
            total_steps_taken_++;
            gs_.take_step(step);
            Step capture_step;
            if (detect_capture_from_motion(gs_, step, &capture_step)) {
                gs_.take_step(capture_step);
            }
            return *this;
        }
        
        const GameState& get_gamestate() const {
            return gs_;
        }
        
        unsigned int get_stepsleft() const {
            return 4 - total_steps_taken_;
        }
        
        const std::string to_std_string() const;
        
        const uint64_t get_zobrist_hash() const {
            return gs_.get_hash();
        }

    private:
        
        const static uint8_t MAXSTEPS = 4;

        GameState gs_;
        Step steps_[MAXSTEPS];
        uint8_t total_steps_taken_;
};

typedef boost::shared_ptr<Move> MovePtr;
typedef std::vector<MovePtr>::const_iterator vec_move_it;

void generate_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves);
void generate_unique_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves);

#endif
