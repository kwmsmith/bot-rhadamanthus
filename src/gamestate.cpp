#include "gamestate.h"
#include <cstdio>
#include <vector>

bool gamestate_from_string(const std::string& str, GameState *gs)
{
    if (str.length() != 64)
        return false;
    int color;
    int piece;
    for (unsigned int idx=0; idx < str.length(); idx++) {
        if (str[idx] == 'x' || str[idx] == 'X' ||
                str[idx] == ' ' || str[idx] == '.')
            continue;
        color = color_from_char(str[idx]);
        piece = piece_from_char(str[idx]);
        if (color == kInvalidPiece || piece == kInvalidPiece)
            return false;
        if (!gs->add_piece_at(color, piece, idx))
            return false;
    }
    // FIXME: Validation!!!
    return true;
}

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
    if (get_all_const().contains(idx)) {
        return false; // position already occupied.
    }
    add_piece_at_fast(c, piece, idx);
    assert(_pieces[piece].contains(idx));
    assert(get_color_board_const(c).contains(idx));
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

    // now we can move the piece.
    remove_piece_at(c, piece, from);
    add_piece_at(c, piece, to);
    return true;
}

bool GameState::is_empty() const
{
    Board b(get_all_const());
    for(int i=R; i < nPieces; ++i)
        b |= _pieces[i];
    return b == 0;
}

Board GameState::has_adjacent_empty(const Color c) const
{
    Board adj_empty;
    const Board empties = ~get_all_const();
    const Board& color_board = get_color_board_const(c);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_empty |= empties.move(direction) & color_board;
    return adj_empty;
}

Board GameState::has_adjacent_friendly(const Color c) const
{
    Board adjacent_friendly;
    const Board& color_board = get_color_board_const(c);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adjacent_friendly |= color_board.move(direction) & color_board;
    return adjacent_friendly;
}

Board GameState::has_adjacent_enemy_le(const Color for_color) const
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

Board GameState::has_adjacent_enemy_lt(const Color for_color) const
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

Board GameState::has_adjacent_enemy_gt(const Color for_color) const
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

Board GameState::has_adjacent_enemy_ge(const Color for_color) const
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

Board GameState::frozen_pieces(const Color c) const
{
    return has_adjacent_enemy_gt(c) & ~has_adjacent_friendly(c);
}

Board GameState::mobile_pieces(const Color c) const
{
    return has_adjacent_empty(c) & ~frozen_pieces(c);
}

void has_adjacent_empty_directional(const GameState& gs, const Color c, std::vector<Board> *boards)
{
    const Board empties = ~gs.get_all_const();
    const Board& color_board = gs.get_color_board_const(c);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction) {
        boards->push_back(empties.move(direction) & color_board);
    }
}

void mobile_pieces_directional(const GameState& gs, const Color c, std::vector<Board> *boards)
{
    Board not_frozen = ~gs.frozen_pieces(c);
    has_adjacent_empty_directional(gs, c, boards);
    for(unsigned int i=0; i < num_directions(); ++i) {
        boards->at(i) &= not_frozen;
    }
}
