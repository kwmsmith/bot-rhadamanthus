#include "gamestate.h"
#include <cstdio>
#include <vector>

std::string GameState::to_std_string() const
{
    const std::string &oneline = to_oneline_string();
    std::string s("+-----------------+\n");
    int rev_row = 0;
    for(int row=0; row < 8; row++) {
        rev_row = 7 - row;
        s += '|';
        s += ' ';
        for(int col=0; col < 8; col++) {
            s += oneline[rev_row * 8 + col];
            s += ' ';
        }
        s += "|\n";
    }
    s += "+-----------------+\n";
    return s;
}

std::string GameState::to_oneline_string() const 
{
    std::string s("................................................................");

    // put x's on the trap squares
    s[18] = 'x'; s[21] = 'x';
    s[42] = 'x'; s[45] = 'x';

    const char white_char[] = { 'R', 'C', 'D', 'H', 'M', 'E'};
    const char black_char[] = { 'r', 'c', 'd', 'h', 'm', 'e'};
    for(int p=R; p < nPieces; p++) {
        const Board &b = _color[W] & _pieces[p];
        const std::vector<unsigned char> &psns = b.psns_from_board();
        for (std::vector<unsigned char>::const_iterator it=psns.begin();
                it != psns.end(); ++it) {
            s[*it] = white_char[p];
        }
    }
    for(int p=R; p < nPieces; p++) {
        const Board &b = _color[B] & _pieces[p];
        const std::vector<unsigned char> &psns = b.psns_from_board();
        for (std::vector<unsigned char>::const_iterator it=psns.begin();
                it != psns.end(); ++it) {
            s[*it] = black_char[p];
        }
    }
    return s;
}

bool GameState::take_step(const Step &s)
{
    if (!s.is_valid())
        return false;

    const int c = s.get_color(), p = s.get_piece(), pos = s.get_position();
    
    assert(c == W || c == B);
    
    switch(s.get_action()) {
        case NORTH:
            return move_piece(c, p, pos, pos+8);
        case SOUTH:
            return move_piece(c, p, pos, pos-8);
        case EAST:
            return move_piece(c, p, pos, pos+1);
        case WEST:
            return move_piece(c, p, pos, pos-1);
        case ADD:
            return add_piece_at(c, p, pos);
        case CAPTURE:
            return remove_piece_at(c, p, pos);
    }
    assert(0); // should never get here.
    return false;
}

bool GameState::add_piece_at(const int c, const int piece, const unsigned int idx)
{
    assert(c == W || c == B);
    assert(piece >= R && piece <= E);
    assert(idx >= 0 && idx < 64);
    if ((_color[W] | _color[B]).contains(idx)) {
        return false; // position already occupied.
    }
    add_piece_at_fast(c, piece, idx);
    assert(_pieces[piece].contains(idx));
    assert(get_color_board_const(c).contains(idx));
    // assert((_pieces[piece] & get_color_board_const(c)).contains(idx));
    return true;
}

bool GameState::remove_piece_at(const int c, const int piece, const unsigned int idx)
{
    if (!(_pieces[piece] & get_color_board_const(c)).contains(idx)) {
        return false; // idx does not contain the right piece.
    }
    remove_piece_at_fast(c, piece, idx);
    assert(!(_pieces[piece] & get_color_board_const(c)).contains(idx));
    return true;
}

/**
 * Safe move.  Returns false on failure, true on success.
 */
bool GameState::move_piece(const int c, const int piece, const unsigned int from, const unsigned int to)
{
    // TODO: Test for move off of board?
    // const int diff = abs(from - to);
    // if(!(diff == 1 || diff == 8)) // not moved in a canonical direction.
        // return false;

    // now we can move the piece.
    remove_piece_at(c, piece, from);
    add_piece_at(c, piece, to);
    return true;
}

bool GameState::is_empty() const
{
    Board b(_color[W] | _color[B]);
    for(int i=R; i < nPieces; ++i)
        b |= _pieces[i];
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
        add_piece_at(c, p, i);
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
    const Board empties = ~(_color[W] | _color[B]);
    const Board& color_board = get_color_board_const(c);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_empty |= empties.move(direction) & color_board;
    return adj_empty;
}

Board GameState::has_adjacent_friendly(Color c) const
{
    Board adjacent_friendly;
    const Board& color_board = get_color_board_const(c);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adjacent_friendly |= color_board.move(direction) & color_board;
    return adjacent_friendly;
}

Board GameState::has_adjacent_enemy_le(Color for_color) const
{
    Board adj_le, enemy_le, these_pieces;
    const Board& color_board = get_color_board_const(for_color);
    const Board& enemy_color = get_color_board_const(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        enemy_le |= _pieces[p] & enemy_color;
        these_pieces = _pieces[p] & color_board;
        for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
            adj_le |= enemy_le.move(direction) & these_pieces;
    }
    return adj_le;
}

Board GameState::has_adjacent_enemy_lt(Color for_color) const
{
    Board adj_lt, enemy_lt, these_pieces;
    const Board& color_board = get_color_board_const(for_color);
    const Board& enemy_color = get_color_board_const(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        these_pieces = _pieces[p] & color_board;
        for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
            adj_lt |= enemy_lt.move(direction) & these_pieces;
        enemy_lt |= _pieces[p] & enemy_color;
    }
    return adj_lt;
}

Board GameState::has_adjacent_enemy_gt(Color for_color) const
{
    Board adj_gt, enemy_gt, these_pieces;
    const Board& color_board = get_color_board_const(for_color);
    const Board& enemy_color = get_color_board_const(other_color(for_color));
    for(int p = E; p >= R; --p) {
        these_pieces = _pieces[p] & color_board;
        for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
            adj_gt |= enemy_gt.move(direction) & these_pieces;
        enemy_gt |= _pieces[p] & enemy_color;
    }
    return adj_gt;
}

Board GameState::has_adjacent_enemy_ge(Color for_color) const
{
    Board adj_ge, enemy_ge, these_pieces;
    const Board& color_board = get_color_board_const(for_color);
    const Board& enemy_color = get_color_board_const(other_color(for_color));
    for(int p = E; p >= R; --p) {
        enemy_ge |= _pieces[p] & enemy_color;
        these_pieces = _pieces[p] & color_board;
        for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
            adj_ge |= enemy_ge.move(direction) & these_pieces;
    }
    return adj_ge;
}

Board GameState::frozen_pieces(Color c) const
{
    return has_adjacent_enemy_gt(c) & ~has_adjacent_friendly(c);
}
