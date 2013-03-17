#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "board.h"
#include "step.h"
#include <iostream>

class GameState {

    public:

        GameState() {
            _color[W].clear(); _color[B].clear();
            for(int i=R; i<nPieces; ++i)
                _pieces[i].clear();
        }

        /**
         * String is in order a1-a8 b1-b8 ... h1-h8, i.e. indices 0..63.
         */
        bool init_from_string(const std::string& s);

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

        const Board& get_piece_board(Piece p) const {
            return _pieces[p];
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

    private:

        GameState(const GameState& gs);  // noncopyable
        GameState& operator=(const GameState& gs);  // noncopyable

        void add_piece_at_fast(const int c, const int piece, const unsigned int idx) {
            if (c == W) {
                _color[W].add(idx);
                assert(_color[W].contains(idx));
            } else {
                _color[B].add(idx);
                assert(_color[B].contains(idx));
            }
            _pieces[piece].add(idx);
            assert(_pieces[piece].contains(idx));
        }

        void remove_piece_at_fast(const int c, const int piece, const unsigned int idx) {
            if (c == W) {
                _color[W].remove(idx);
            } else {
                _color[B].remove(idx);
            }
            _pieces[piece].remove(idx);
        }

        Board _color[2];
        Board _pieces[nPieces];
};

#endif
