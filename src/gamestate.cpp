#include "gamestate.h"
#include <cstdio>

void GameState::add_piece_at(int piece, int c, unsigned int idx)
{
    Board &color = (c == W ? _white : _black);
    color.add(idx);
    _boards[piece].add(idx);
}

void GameState::remove_piece_at(int piece, int c, unsigned int idx)
{
    Board &color = (c == W ? _white : _black);
    color.remove(idx);
    _boards[piece].remove(idx);
}

/**
 * Safe move.  Returns 0 on failure, 1 on success.
 */
int GameState::move_piece(int piece, int c, unsigned int from, unsigned int to)
{
    Board all_pieces = _white | _black;
    if(all_pieces.contains(to)) // destination is occupied.
        return 0;
    Board &color = (c == W ? _white : _black);
    if(!(_boards[piece] & color).contains(from)) // specified piece is not here.
        return 0;
    int diff = abs(from - to);
    if(!(diff == 1 || diff == 8)) // not moved in a canonical direction.
        return 0;

    // now we can move the piece.
    remove_piece_at(piece, c, from);
    add_piece_at(piece, c, to);
    return 1;
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
    return has_adjacent_empty(c) & ~frozen_pieces(c);
}

Board GameState::has_adjacent_empty(Color c) const
{
    Board adj_empty;
    const Board empties = ~(_white | _black);
    const Board& color_board = get_color_board(c);
    for(int direction = NORTH; direction < kNumDirections; ++direction)
        adj_empty |= empties.move(direction) & color_board;
    return adj_empty;
}

Board GameState::has_adjacent_friendly(Color c) const
{
    Board adjacent_friendly;
    const Board& color_board = get_color_board(c);
    for(int direction = NORTH; direction < kNumDirections; ++direction)
        adjacent_friendly |= color_board.move(direction) & color_board;
    return adjacent_friendly;
}

Board GameState::has_adjacent_enemy_le(Color for_color) const
{
    Board adj_le, enemy_le, these_pieces;
    const Board& color_board = get_color_board(for_color);
    const Board& enemy_color = get_color_board(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        enemy_le |= _boards[p] & enemy_color;
        these_pieces = _boards[p] & color_board;
        for(int direction = NORTH; direction < kNumDirections; ++direction)
            adj_le |= enemy_le.move(direction) & these_pieces;
    }
    return adj_le;
}

Board GameState::has_adjacent_enemy_lt(Color for_color) const
{
    Board adj_lt, enemy_lt, these_pieces;
    const Board& color_board = get_color_board(for_color);
    const Board& enemy_color = get_color_board(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        these_pieces = _boards[p] & color_board;
        for(int direction = NORTH; direction < kNumDirections; ++direction)
            adj_lt |= enemy_lt.move(direction) & these_pieces;
        enemy_lt |= _boards[p] & enemy_color;
    }
    return adj_lt;
}

Board GameState::has_adjacent_enemy_gt(Color for_color) const
{
    Board adj_gt, enemy_gt, these_pieces;
    const Board& color_board = get_color_board(for_color);
    const Board& enemy_color = get_color_board(other_color(for_color));
    for(int p = E; p >= R; --p) {
        these_pieces = _boards[p] & color_board;
        for(int direction = NORTH; direction < kNumDirections; ++direction)
            adj_gt |= enemy_gt.move(direction) & these_pieces;
        enemy_gt |= _boards[p] & enemy_color;
    }
    return adj_gt;
}

Board GameState::has_adjacent_enemy_ge(Color for_color) const
{
    Board adj_ge, enemy_ge, these_pieces;
    const Board& color_board = get_color_board(for_color);
    const Board& enemy_color = get_color_board(other_color(for_color));
    for(int p = E; p >= R; --p) {
        enemy_ge |= _boards[p] & enemy_color;
        these_pieces = _boards[p] & color_board;
        for(int direction = NORTH; direction < kNumDirections; ++direction)
            adj_ge |= enemy_ge.move(direction) & these_pieces;
    }
    return adj_ge;
}

Board GameState::frozen_pieces(Color c) const
{
    return has_adjacent_enemy_gt(c) & ~has_adjacent_friendly(c);
}
