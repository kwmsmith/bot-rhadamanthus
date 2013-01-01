#include "gamestate.h"
#include <cstdio>

void GameState::add_piece_at(int piece, int c, unsigned int idx)
{
    Board &color = (c == W ? _white : _black);
    color.add(idx);
    _boards[piece].add(idx);
}

bool GameState::is_empty() const
{
    Board b(_white);
    b |= _black;
    for(int i=R; i < nPieces; ++i)
        b |= _boards[i];
    return b == 0;
}

bool GameState::init_from_string(const std::string& s)
{
    int p = 0, c = 0;
    if (s.length() != 64)
        return false;
    for(unsigned int i=0; i < s.length(); ++i) {
        p = piece_from_char(s[i]);
        c = color_from_char(s[i]);
        if (p == kInvalidPiece || c == kInvalidPiece)
            continue;
        add_piece_at(p, c, i);
    }
    return true;
}

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
    Board frozen_pieces, these_pieces;
    Board enemies_stronger[nPieces];
    const Board& color_board = get_color_board(c);
    pieces_stronger(other_color(c), enemies_stronger);
    for(int p = R; p < nPieces; ++p) {
        these_pieces = _boards[p] & color_board;
        for(int direction = NORTH; direction < kNumDirections; ++direction)
            frozen_pieces |= enemies_stronger[p].move(direction) & these_pieces;
    }
    return frozen_pieces;
}

void GameState::pieces_stronger(Color for_color, Board pieces_stronger[nPieces]) const
{
    Board color_pieces = (for_color == W ? _white : _black);
    Board stronger_accum(0);
    for(int p = E; p >= R; --p) {
        pieces_stronger[p] = stronger_accum;
        stronger_accum |= _boards[p] & color_pieces;
    }
}
