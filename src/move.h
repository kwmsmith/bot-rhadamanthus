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
            assert(steps_.size() <= 3);
            steps_.push_back(step);
            gs_.take_step(step);
            return *this;
        }
        
        const GameState& get_gamestate() const {
            return gs_;
        }
        
        unsigned int get_stepsleft() const {
            return 4 - steps_.size();
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
