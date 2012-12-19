#include "board.h"
#include <iostream>

enum Piece { R, C, D, H, M, E, nPieces };

enum Color { W, B };

inline Color other_color(Color c) {
    return c == W ? B : W;
}

class GameState {

    public:

        GameState() : _white(0), _black(0) {}

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

        /**
         * 
         */
        void pieces_stronger(Color for_color, Board pieces_stronger[nPieces]) const;

        Board *pieces_weaker(Color for_color) const;

    private:
        Board _white;
        Board _black;
        Board _boards[nPieces];
};
