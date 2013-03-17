#ifndef STEP_H_
#define STEP_H_

#include "board.h"

inline unsigned int invalid_color() {return 100;}
inline unsigned int invalid_piece() {return 100;}
inline unsigned int invalid_position() { return 100;}
inline unsigned int invalid_action() { return 100;}

int action_from_char(const char ch);

bool parse_action_str(const std::string& ss, 
        unsigned char *color, unsigned char *piece, unsigned char *position, unsigned char *action);

class Step
{
    public:

        Step(unsigned char color, unsigned char piece, unsigned char position, unsigned char action)
            :color_(color),
             piece_(piece),
             position_(position),
             action_(action) {}

        Step(const Step &m)
            :color_(m.color_),
             piece_(m.piece_),
             position_(m.position_),
             action_(m.action_) {}

        Step &operator=(const Step &other) {
            color_ = other.color_;
            piece_ = other.piece_;
            position_ = other.position_;
            action_ = other.action_;
            return *this;
        }

        bool is_move() const {
            return (action_ == NORTH || action_ == SOUTH || action_ == EAST || action_ == WEST);
        }

        bool is_capture() const {
            return action_ == CAPTURE;
        }

        bool is_placement() const {
            return action_ == ADD;
        }

        bool is_valid() const {
            if (color_ == invalid_color() ||
                    piece_ == invalid_piece() ||
                    position_ == invalid_position() ||
                    action_ == invalid_action()) {
                return false;
            }
            
            if (!(color_ == W || color_ == B))
                return false;
            if (piece_ > E)
                return false;
            if (position_ > 63)
                return false;
            if (action_ > CAPTURE)
                return false;
            
            switch(action_) {
                case NORTH:
                    if (Board::rank_n(8).contains(position_))
                        return false;
                    break;
                case SOUTH:
                    if (Board::rank_n(1).contains(position_))
                        return false;
                    break;
                case EAST:
                    if (Board::file_n(8).contains(position_))
                        return false;
                    break;
                case WEST:
                    if (Board::file_n(1).contains(position_))
                        return false;
                    break;
                case CAPTURE:
                    if (!Board::capture_squares().contains(position_))
                        return false;
                    break;
            }
            return true;
        }

        unsigned char get_color() const {
            return color_;
        }

        unsigned char get_piece() const {
            return piece_;
        }

        unsigned char get_position() const {
            return position_;
        }

        unsigned char get_action() const {
            return action_;
        }

    private:
        unsigned char color_, piece_, position_, action_;
};

inline Step make_step(const std::string& ss)
{
    unsigned char color = invalid_color(), piece = invalid_piece(), 
                  pos = invalid_position(), action = invalid_action();
    parse_action_str(ss, &color, &piece, &pos, &action);
    return Step(color, piece, pos, action);
}

#endif // STEP_H_
