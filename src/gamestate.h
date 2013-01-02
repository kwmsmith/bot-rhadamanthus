#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "board.h"
#include <iostream>

const static int kInvalidPiece = -1;

enum Piece { R, C, D, H, M, E, nPieces };

enum Color { W, B };

inline Color other_color(Color c) {
    return c == W ? B : W;
}

/**
 * Takes a display integer (0 -> square h1, 63 -> square a8), and converts it
 * to its internal index.
 */
inline int internal_idx_from_display(int d)
{
    int row =  - d / 8 + 7;
    int col = d % 8;
    return 8 * row + col;
}

inline int color_from_char(char ch)
{
    switch(ch) {
        case 'R':
        case 'C':
        case 'D':
        case 'H':
        case 'M':
        case 'E':
            return W;
        case 'r':
        case 'c':
        case 'd':
        case 'h':
        case 'm':
        case 'e':
            return B;
        default:
            return kInvalidPiece;
    }
}

inline int piece_from_char(char ch)
{
    switch(ch) {
        case 'R':
        case 'r':
            return R;
        case 'C':
        case 'c':
            return C;
        case 'D':
        case 'd':
            return D;
        case 'H':
        case 'h':
            return H;
        case 'M':
        case 'm':
            return M;
        case 'E':
        case 'e':
            return E;
        default:
            return kInvalidPiece;
    }
}

class GameState {

    public:

        GameState() : _white(0), _black(0) {}

        /**
         * String is in order a1-a8 b1-b8 ... h1-h8, i.e. indices 0..63.
         */
        bool init_from_string(const std::string& s);

        void clear() {
            _white.clear(); _black.clear();
            for(int i=R; i<nPieces; ++i)
                _boards[i].clear();
        }

        void add_piece_at(int p, int c, unsigned int idx);

        void add_piece_at(int p, int c, char file, int rank) {
            assert(file >= 'A' && file <= 'H');
            assert(rank >= 1 && rank <= 8);
            add_piece_at(p, c, (rank - 1) * 8 + (file - 'A'));
        }

        void remove_piece_at(int p, int c, unsigned int idx);

        int move_piece(int p, int c, unsigned int from, unsigned int to);

        const Board& get_color_board(Color c) const {
            return (c == B ? _black : _white);
        }

        const Board& get_piece_board(Piece p) const {
            return _boards[p];
        }

        bool contains_at(int p, Color c, unsigned int idx) const {
            const Board& color = get_color_board(c);
            return color.contains(idx) && _boards[p].contains(idx);
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

    private:

        GameState(const GameState& gs);
        GameState& operator=(const GameState& gs);

        Board _white;
        Board _black;
        Board _boards[nPieces];
};

#endif

