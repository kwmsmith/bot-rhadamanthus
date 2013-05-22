#ifndef _MOVE_H_
#define _MOVE_H_

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

        Move(const GameState& gs) : gs_(gs), total_steps_taken_(0) {}
        
        Move& add_step(const Step& step) {
            assert(step.is_motion());
            assert(get_stepsleft() >= 1);
            steps_[total_steps_taken_] = step;
            total_steps_taken_++;
            gs_.take_step(step);
            Step capture_step;
            if (possible_capture_from_motion(gs_, step)) {
                capture_from_motion(step, &gs_);
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

void generate_moves(const GameState& gs, const Color for_color, std::vector<Move> *moves);
void generate_unique_moves(const GameState& gs, const Color for_color, std::vector<Move> *moves);

#endif
