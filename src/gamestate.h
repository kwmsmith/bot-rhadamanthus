#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "board.h"
#include "step.h"
#include "transposition.h"

class GameState 
{

    public:

        GameState() {
            clear();
        }

        void clear() {
            _color[W].clear(); _color[B].clear();
            for(int i=R; i<nPieces; ++i)
                _pieces[i].clear();
            _zhash.clear();
        }

        bool take_step(const Step &s);

        bool move_piece(const int c, const int p, const uint8_t from, const uint8_t to);

        const Board& get_color_board_const(const int c) const {
            assert(c == B || c == W);
            return _color[c];
        }
        
        const Board& get_piece_board_const(const int p) const {
            return _pieces[p];
        }
        
        const uint8_t get_count(const int p, const int c) const {
            return (get_piece_board_const(p) & get_color_board_const(c)).count();
        }

        const Board get_all_const() const {
            return _color[W] | _color[B];
        }

        bool contains_at(const Color c, const int p, const uint8_t idx) const {
            const Board& color = get_color_board_const(c);
            return color.contains(idx) && _pieces[p].contains(idx);
        }

        bool is_empty() const;

        bool add_piece_at(const int c, const int p, const uint8_t idx);

        bool add_piece_at(const int c, const int p, const char file, const int rank) {
            assert(file >= 'A' && file <= 'H');
            assert(rank >= 1 && rank <= 8);
            return add_piece_at(c, p, (rank - 1) * 8 + (file - 'A'));
        }

        bool remove_piece_at(const int c, const int p, const uint8_t idx);
        
        std::string to_oneline_string(const char empty='.') const;

        std::string to_std_string(const char empty='.') const;
        
        void apply_mask(const Board& mask) {
            _color[W] &= mask; _color[B] &= mask;
            for(int i=R; i<nPieces; ++i)
                _pieces[i] &= mask;
        }
        
        void copy_to(GameState *to) const {
            *to = *this;
        }
        
        void color_and_piece_at(const uint8_t idx, int *c, int *p) const {
            int8_t _c = -1, _p = -1; // guilty before proven innocent...
            
            _c += _color[W].contains(idx) * (W+1);
            _c += _color[B].contains(idx) * (B+1);

            const Board bitidx = Board().add(idx);
            for(unsigned int i=0; i < nPieces; ++i)
                _p += (_pieces[i] & bitidx).any() * (i+1);
            
            assert(_c >= 0 && _c <= 1);
            assert(_p >= 0 && _p < nPieces);
            *c = _c; *p = _p;
        }
        
        const uint64_t get_hash() const { return _zhash.get_hash(); }

    private:

        void add_piece_at_fast(const int c, const int piece, const uint8_t idx) {
            assert (c == W || c == B);
            _color[c].add(idx);
            assert(_color[c].contains(idx));
            _pieces[piece].add(idx);
            assert(_pieces[piece].contains(idx));
            _zhash.addrm_piece_at(c, piece, idx);
        }

        void remove_piece_at_fast(const int c, const int piece, const uint8_t idx) {
            assert (c == W || c == B);
            _color[c].remove(idx);
            _pieces[piece].remove(idx);
            _zhash.addrm_piece_at(c, piece, idx);
        }

        Board _color[2];
        Board _pieces[nPieces];
        ZobristHash _zhash;
};

void generate_pushes(const GameState& gs, const Color for_color, std::vector<Step> *pushes);
void generate_pulls(const GameState& gs, const Color for_color, std::vector<Step> *pulls);
void generate_steps(const GameState& gs, const Color for_color, std::vector<Step> *steps);
unsigned char generate_captures(const GameState& gs, std::vector<Step> *captures);

bool detect_capture_from_motion(const GameState& gs, const Step& step_taken, Step *capture);
bool possible_capture_from_motion(const GameState &gs, const Step& step_taken);
void capture_from_motion(const Step& step_taken, GameState *gs);

Step step_from_gs(const GameState& gs, const uint8_t idx, const unsigned int direction);

/* Piece characters are rcdhme / RCDHME.  Empty characters are '.', and ' ' and
 * are ignored.  Trap characters ('x' or 'X') are ignored.
 */
bool gamestate_from_input(const std::string& ss, GameState *gs, Color *to_move);
bool gamestate_from_oneline(const std::string& ss, GameState *gs, Color *to_move);

void mobile_pieces_directional(const GameState& gs, const Color c, std::vector<Board> *boards);
void has_adjacent_empty_directional(const GameState& gs, const Color c, std::vector<Board> *boards);

Board adj_enemy_gt(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_enemy_gt(const GameState& gs, const Color for_color);

Board adj_enemy_ge(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_enemy_ge(const GameState& gs, const Color for_color);

Board adj_enemy_lt(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_enemy_lt(const GameState& gs, const Color for_color);

Board adj_enemy_le(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_enemy_le(const GameState& gs, const Color for_color);

Board adj_step(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_step(const GameState& gs, const Color for_color);

Board adj_empty(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_empty(const GameState& gs, const Color for_color);

Board adj_friendly(const GameState& gs, const Color for_color, const unsigned int direction);
Board adj_friendly(const GameState& gs, const Color for_color);

Board frozen_pieces(const GameState& gs, const Color for_color);
Board mobile_pieces(const GameState& gs, const Color for_color);

void get_num_pieces_array(const GameState& gs, const Color for_color, uint8_t *num_pieces);

#endif
