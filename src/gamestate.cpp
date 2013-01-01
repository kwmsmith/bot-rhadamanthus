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

Board GameState::adjacent_friendly(Color c) const
{
    Board adjacent_friendly;
    const Board& color_board = get_color_board(c);
    for(int direction = NORTH; direction < kNumDirections; ++direction)
        adjacent_friendly |= color_board.move(direction) & color_board;
    return adjacent_friendly;
}

Board GameState::adjacent_stronger(Color c) const
{
    Board these_pieces, adjacent_stronger;
    Board enemies_stronger[nPieces];
    const Board& color_board = get_color_board(c);
    pieces_stronger(other_color(c), enemies_stronger);
    for(int p = R; p < nPieces; ++p) {
        these_pieces = _boards[p] & color_board;
        for(int direction = NORTH; direction < kNumDirections; ++direction)
            adjacent_stronger |= enemies_stronger[p].move(direction) & these_pieces;
    }
    return adjacent_stronger;
}

Board GameState::frozen_pieces(Color c) const
{
    return adjacent_stronger(c) & ~adjacent_friendly(c);
}

void GameState::pieces_stronger(Color for_color, Board pieces_stronger[nPieces]) const
{
    Board color_pieces = get_color_board(for_color);
    Board stronger_accum(0);
    for(int p = E; p >= R; --p) {
        pieces_stronger[p] = stronger_accum;
        stronger_accum |= _boards[p] & color_pieces;
    }
}
