#include "step.h"
#include "gamestate.h"
#include <boost/shared_ptr.hpp>

class Move
{
    public:

        Move(const GameState& gs) {
            gs.copy_to(&gs_); 
        }
        
        Move(const Move& m) {
            steps_ = m.steps_;
            m.gs_.copy_to(&gs_);
        }
        
        Move& add_step(const Step& step) {
            assert(step.is_motion());
            assert(get_stepsleft() >= 1);
            steps_.push_back(step);
            gs_.take_step(step);
            Step capture_step;
            const bool is_capture = detect_capture_from_motion(gs_, step, &capture_step);
            if (is_capture) {
                steps_.push_back(capture_step);
                gs_.take_step(capture_step);
            }
            return *this;
        }
        
        const GameState& get_gamestate() const {
            return gs_;
        }
        
        unsigned int get_stepsleft() const {
            typedef std::vector<Step>::const_iterator step_it;
            signed char motion_left = 4;
            for(step_it it=steps_.begin(); it != steps_.end(); ++it)
                motion_left -= it->cost();
            assert(motion_left >= 0);
            return motion_left;
        }

    private:

        GameState gs_;
        std::vector<Step> steps_;
};

typedef boost::shared_ptr<Move> MovePtr;

/**
 * Takes a gamestate, generates all unique moves from this gamestate.
 */
void generate_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves);
