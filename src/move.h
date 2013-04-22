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
                assert(capture_step.is_capture());
                steps_.push_back(capture_step);
                gs_.take_step(capture_step);
            }
            return *this;
        }
        
        const GameState& get_gamestate() const {
            return gs_;
        }
        
        unsigned int get_stepsleft() const {
            signed char motion_left = 4;
            for(step_it it=steps_.begin(); it != steps_.end(); ++it)
                motion_left -= it->cost();
            assert(motion_left >= 0);
            return motion_left;
        }
        
        const std::string to_std_string() const;
        
        const uint64_t get_zobrist_hash() const {
            return gs_.get_hash();
        }

    private:

        GameState gs_;
        std::vector<Step> steps_;
};

typedef boost::shared_ptr<Move> MovePtr;

void generate_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves);
void generate_unique_moves(const GameState& gs, const Color for_color, std::vector<MovePtr> *moves);
