#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "board.h"
#include "step.h"
#include "transposition.h"

class GameState 
{

  public:

    //  Only default ctor defined.
    GameState() : _color{0ULL, 0ULL},
      _pieces{0ULL,0ULL,0ULL,0ULL,0ULL,0ULL},
      _zhash(),
      _this_color(W),
      _stepsleft(4) 
      { }

    // Everthing else is explicitly default since a GameState has no handles.
    GameState(const GameState&) = default;
    GameState(GameState&&) = default;
    GameState& operator=(const GameState&) = default;
    GameState& operator=(GameState&&) = default;
    ~GameState() = default;

    void clear() {
      _color[W].clear(); _color[B].clear();
      for(int i=R; i<nPieces; ++i)
        _pieces[i].clear();
      _this_color = W; _stepsleft = 4;
      _zhash.clear();
    }

    bool move_piece(const Step &s);
    bool move_piece(const uint8_t c, const uint8_t p, const uint8_t from, const uint8_t to);

    const uint8_t get_color() const {
      return _this_color;
    }

    void set_color(const int c) {
      if (c == _this_color)
        return;
      flip_color();
    }

    void flip_color() {
      _stepsleft = 4;
      _this_color = other_color(_this_color);
      _zhash.flip_color();
    }

    const uint8_t get_stepsleft() const {
      return _stepsleft;
    }

    const Board& get_color_board(const int c) const {
      assert(c == B || c == W);
      return _color[c];
    }

    const Board& get_piece_board(const int p) const {
      return _pieces[p];
    }

    const uint8_t get_count(const int p, const int c) const {
      return (get_piece_board(p) & get_color_board(c)).count();
    }

    const Board get_all_const() const {
      return _color[W] | _color[B];
    }

    bool contains_at(const uint8_t c, const uint8_t p, const uint8_t idx) const {
      return get_color_board(c).contains(idx) && _pieces[p].contains(idx);
    }

    bool is_empty() const;

    bool add_piece_at(const Step &s);
    bool add_piece_at(const uint8_t c, const uint8_t p, const uint8_t idx);

    bool add_piece_at(const uint8_t c, const uint8_t p, const char file, const int rank) {
      assert(file >= 'A' && file <= 'H');
      assert(rank >= 1 && rank <= 8);
      return add_piece_at(c, p, (rank - 1) * 8 + (file - 'A'));
    }

    bool remove_piece_at(const uint8_t c, const uint8_t p, const uint8_t idx);

    std::string to_oneline_string(const char empty='.') const;

    std::string to_std_string(const char empty='.') const;

    void apply_mask(const Board& mask) {
      _color[W] &= mask; _color[B] &= mask;
      for(int i=R; i<nPieces; ++i)
        _pieces[i] &= mask;
    }

    void color_and_piece_at(const uint8_t idx, int8_t *c, int8_t *p) const {
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

    void add_piece_at_fast(const uint8_t c, const uint8_t piece, const uint8_t idx) {
      assert (c == W || c == B);
      _color[c].add(idx);
      assert(_color[c].contains(idx));
      _pieces[piece].add(idx);
      assert(_pieces[piece].contains(idx));
      _zhash.addrm_piece_at(c, piece, idx);
    }

    void remove_piece_at_fast(const uint8_t c, const uint8_t piece, const uint8_t idx) {
      assert (c == W || c == B);
      _color[c].remove(idx);
      _pieces[piece].remove(idx);
      _zhash.addrm_piece_at(c, piece, idx);
    }

    Board _color[2];
    Board _pieces[nPieces];
    ZobristHash _zhash;
    uint8_t _this_color, _stepsleft;
};

void generate_pushes(const GameState& gs, std::vector<Delta> *pushes);
void generate_pulls(const GameState& gs, std::vector<Delta> *pulls);
void generate_steps(const GameState& gs, std::vector<Delta> *steps);

void take_step_and_capture(const Step& step, GameState *gs);
void apply_delta_and_capture(const Delta& dd, GameState *gs);

bool detect_capture_from_motion(const GameState& gs, const Step& step_taken, Step *capture);
bool possible_capture_from_motion(const GameState &gs, const Step& step_taken);
void capture_from_motion(const Step& step_taken, GameState *gs);

Step step_from_gs(const GameState& gs, const uint8_t idx, const unsigned int direction);

/* Piece characters are rcdhme / RCDHME.  Empty characters are '.', and ' ' and
 * are ignored.  Trap characters ('x' or 'X') are ignored.
 */
bool gamestate_from_input(const std::string& ss, GameState *gs);
bool gamestate_from_oneline(const std::string& ss, GameState *gs);
GameState gamestate_from_goal_position(const std::string& ss, int*);


Board adj_enemy_gt(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_enemy_gt(const GameState& gs, const uint8_t for_color);

Board adj_enemy_ge(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_enemy_ge(const GameState& gs, const uint8_t for_color);

Board adj_enemy_lt(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_enemy_lt(const GameState& gs, const uint8_t for_color);

Board adj_enemy_le(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_enemy_le(const GameState& gs, const uint8_t for_color);

Board adj_step(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_step(const GameState& gs, const uint8_t for_color);

Board adj_empty(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_empty(const GameState& gs, const uint8_t for_color);

Board adj_friendly(const GameState& gs, const uint8_t for_color, const unsigned int direction);
Board adj_friendly(const GameState& gs, const uint8_t for_color);

Board frozen_pieces(const GameState& gs, const uint8_t for_color);
Board mobile_pieces(const GameState& gs, const uint8_t for_color);

void get_num_pieces_array(const GameState& gs, const uint8_t for_color, uint8_t *num_pieces);

// void mobile_pieces_directional(const GameState& gs, const uint8_t c, std::vector<Board> *boards);
// void has_adjacent_empty_directional(const GameState& gs, const uint8_t c, std::vector<Board> *boards);

#endif
