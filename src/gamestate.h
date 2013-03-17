#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "board.h"
#include "step.h"
#include <iostream>

class GameState {

    public:

        GameState() {
            clear();
        }

        void clear() {
            _color[W].clear(); _color[B].clear();
            for(int i=R; i<nPieces; ++i)
                _pieces[i].clear();
        }

        bool take_step(const Step &s);

        bool move_piece(const int c, const int p, const unsigned int from, const unsigned int to);

        const Board& get_color_board_const(const int c) const {
            assert(c == B || c == W);
            return _color[c];
        }
        
        const Board& get_piece_board_const(Piece p) const {
            return _pieces[p];
        }

        const Board get_all_const() const {
            return _color[W] | _color[B];
        }

        bool contains_at(Color c, int p, unsigned int idx) const {
            const Board& color = get_color_board_const(c);
            return color.contains(idx) && _pieces[p].contains(idx);
        }


        Board has_adjacent_empty(Color c) const;

        Board has_adjacent_friendly(Color c) const;

        Board has_adjacent_enemy_le(Color c) const;

        Board has_adjacent_enemy_lt(Color c) const;

        Board has_adjacent_enemy_ge(Color c) const;

        Board has_adjacent_enemy_gt(Color c) const;

        /**
         * Pieces of color `c` that can move without pushing / pulling.
         */
        Board mobile_pieces(Color c) const;

        /**
         * Pieces of color `c` that are not frozen but blockaded.
         */
        Board blockaded_pieces(Color c) const;

        /**
         * Union of blockaded and frozen pieces.
         */
        Board immobile_pieces(Color c) const;

        /**
         * Pieces of color `c` with an adjacent stronger enemy piece.
         */
        Board frozen_pieces(Color c) const;

        bool is_empty() const;

        bool add_piece_at(const int c, const int p, const unsigned int idx);

        void add_piece_at(const int c, const int p, const char file, const int rank) {
            assert(file >= 'A' && file <= 'H');
            assert(rank >= 1 && rank <= 8);
            add_piece_at(c, p, (rank - 1) * 8 + (file - 'A'));
        }

        bool remove_piece_at(const int c, const int p, const unsigned int idx);

        std::string to_oneline_string() const;

        std::string to_std_string() const;
        
        void apply_mask(const Board& mask) {
            _color[W] &= mask; _color[B] &= mask;
            for(int i=R; i<nPieces; ++i)
                _pieces[i] &= mask;
        }
        
        void copy_to(GameState *to) const {
            to->_color[W] = _color[W];
            to->_color[B] = _color[B];
            for(int i=R; i<nPieces; ++i)
                to->_pieces[i] = _pieces[i];
        }

    private:

        GameState(const GameState& gs);  // noncopyable
        GameState& operator=(const GameState& gs);  // noncopyable

        void add_piece_at_fast(const int c, const int piece, const unsigned int idx) {
            assert (c == W || c == B);
            _color[c].add(idx);
            assert(_color[c].contains(idx));
            _pieces[piece].add(idx);
            assert(_pieces[piece].contains(idx));
        }

        void remove_piece_at_fast(const int c, const int piece, const unsigned int idx) {
            assert (c == W || c == B);
            _color[c].remove(idx);
            _pieces[piece].remove(idx);
        }

        Board _color[2];
        Board _pieces[nPieces];
};


/* Expects a 64-character string, beginning with a1, ending with h8.  Piece
 * characters are rcdhme / RCDHME.  Empty characters are '.', ' ', '~' and are
 * ignored.  Trap characters ('x' or 'X') are ignored.
 */
bool gamestate_from_string(const std::string& str, GameState *gs);

void mobile_pieces_directional(const GameState& gs, const Color c, std::vector<Board> *boards);
void has_adjacent_empty_directional(const GameState& gs, const Color c, std::vector<Board> *boards);

#endif
