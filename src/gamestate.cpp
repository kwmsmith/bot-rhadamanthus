#include "gamestate.h"

Board GameState::mobile_pieces(Color c) const
{
    Board mobile_pieces;

    // Define Board array "enemies_weaker" such that
    // enemies_weaker[R] is an empty board,
    // enemies_weaker[C] is a Board of the M,E positions,
    // ...  enemy_stronger_than[R] is a Board of the C,D,H,M,E enemy positions.

    // for each direction
        // move all pieces of this color in that direction
        // select moved-to squares that are empty or
        //    that are not stronger enemy
    return mobile_pieces;
}

Board GameState::frozen_pieces(Color c) const
{
    Board frozen_pieces;
    Board enemies_stronger[nPieces];
    pieces_stronger(other_color(c), enemies_stronger);
    for(int p = R; p < nPieces; ++p) {

    }
    return frozen_pieces;
}

void GameState::pieces_stronger(Color for_color, Board pieces_stronger[nPieces]) const
{
    Board color_pieces = (for_color == W ? _white : _black);
    Board stronger_accum;
    for(int p = E; p >= R; --p) {
        pieces_stronger[p] = stronger_accum;
        stronger_accum = stronger_accum | _boards[p];
    }
}
