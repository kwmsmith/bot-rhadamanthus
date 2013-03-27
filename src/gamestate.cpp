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
        const std::vector<unsigned int> &psns = b.psns_from_board();
        for (std::vector<unsigned int>::const_iterator it=psns.begin();
                it != psns.end(); ++it) {
            s[*it] = white_char[p];
        }
    }
    for(int p=R; p < nPieces; p++) {
        const Board &b = _color[B] & _pieces[p];
        const std::vector<unsigned int> &psns = b.psns_from_board();
        for (std::vector<unsigned int>::const_iterator it=psns.begin();
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

void has_adjacent_empty_directional(const GameState& gs, const Color c, std::vector<Board> *boards)
{
    const Board empties = ~gs.get_all_const();
    const Board& color_board = gs.get_color_board_const(c);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction) {
        boards->push_back(is_adjacent(color_board, empties, direction));
    }
}

void mobile_pieces_directional(const GameState& gs, const Color c, std::vector<Board> *boards)
{
    Board not_frozen = ~frozen_pieces(gs, c);
    has_adjacent_empty_directional(gs, c, boards);
    for(unsigned int i=0; i < num_directions(); ++i) {
        boards->at(i) &= not_frozen;
    }
}

void generate_pushes(const GameState& gs, const Color for_color, std::vector<std::vector<Step> > *pushes)
{
    const Board& mobile = mobile_pieces(gs, for_color);
    for (unsigned int pusher_direction = NORTH; pusher_direction < num_directions(); ++pusher_direction) {
        const Board& pieces_with_adj_lt = adj_enemy_lt(gs, for_color, pusher_direction) & mobile;
        for (unsigned int pushed_direction = NORTH; pushed_direction < num_directions(); ++pushed_direction) {
            // if (pusher_direction == pushed_direction) continue;
            const Board& enemy_with_adj_empty = adj_empty(gs, other_color(for_color), pushed_direction);
                
            const Board& pushed_pieces = is_adjacent(enemy_with_adj_empty,
                                                     pieces_with_adj_lt, opp_dir(pusher_direction));
            const Board& pushing_pieces = is_adjacent(pieces_with_adj_lt,
                                                      enemy_with_adj_empty, pusher_direction);
            std::vector<unsigned int> pushed_idxs, pusher_idxs;
            pushed_pieces.psns_from_board(&pushed_idxs);
            pushing_pieces.psns_from_board(&pusher_idxs);
            assert(pushed_idxs.size() == pusher_idxs.size());
            for(unsigned int i=0; i < pushed_idxs.size(); ++i) {
                assert(gs.get_all_const().contains(pushed_idxs[i]));
                assert(gs.get_all_const().contains(pusher_idxs[i]));
                std::vector<Step> delta;
                delta.push_back(step_from_gs(gs, pushed_idxs[i], pushed_direction));
                delta.push_back(step_from_gs(gs, pusher_idxs[i], pusher_direction));
                pushes->push_back(delta);
            }
        }
    }
}

Board adj_enemy_gt(const GameState& gs, const Color for_color, const unsigned int direction)
{
    Board adj_gt, enemy_gt, these_pieces;
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board& enemy_color = gs.get_color_board_const(other_color(for_color));
    for(int p = E; p >= R; --p) {
        these_pieces = gs.get_piece_board_const(p) & color_board;
        adj_gt |= is_adjacent(these_pieces, enemy_gt, direction);
        enemy_gt |= gs.get_piece_board_const(p) & enemy_color;
    }
    return adj_gt;
}

Board adj_enemy_ge(const GameState& gs, const Color for_color, const unsigned int direction)
{
    Board adj_ge, enemy_ge, these_pieces;
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board& enemy_color = gs.get_color_board_const(other_color(for_color));
    for(int p = E; p >= R; --p) {
        enemy_ge |= gs.get_piece_board_const(p) & enemy_color;
        these_pieces = gs.get_piece_board_const(p) & color_board;
        adj_ge |= is_adjacent(these_pieces, enemy_ge, direction);
    }
    return adj_ge;
}

Board adj_enemy_lt(const GameState& gs, const Color for_color, const unsigned int direction)
{
    Board adj_lt, enemy_lt, these_pieces;
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board& enemy_color = gs.get_color_board_const(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        these_pieces = gs.get_piece_board_const(p) & color_board;
        std::cout << "these_pieces: " << p << ' ' << these_pieces.to_string() << std::endl;
        std::cout << "enemy_lt    : " << p << ' ' << enemy_lt.to_string() << std::endl;
        adj_lt |= is_adjacent(these_pieces, enemy_lt, direction);
        std::cout << "adj_lt      : " << p << ' ' << adj_lt.to_string() << std::endl;
        enemy_lt |= gs.get_piece_board_const(p) & enemy_color;
    }
    return adj_lt;
}

Board adj_enemy_le(const GameState& gs, const Color for_color, const unsigned int direction)
{
    Board adj_le, enemy_le, these_pieces;
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board& enemy_color = gs.get_color_board_const(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        enemy_le |= gs.get_piece_board_const(p) & enemy_color;
        these_pieces = gs.get_piece_board_const(p) & color_board;
        adj_le |= is_adjacent(these_pieces, enemy_le, direction);
    }
    return adj_le;
}

Board adj_empty(const GameState& gs, const Color for_color)
{
    Board adj_mt;
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board empties = ~gs.get_all_const();
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_mt |= is_adjacent(color_board, empties, direction);
    return adj_mt;
}

Board adj_empty(const GameState& gs, const Color for_color, const unsigned int direction)
{
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board empties = ~gs.get_all_const();
    return is_adjacent(color_board, empties, direction);
}

Board adj_friendly(const GameState& gs, const Color for_color, const unsigned int direction)
{
    const Board& color_board = gs.get_color_board_const(for_color);
    return is_adjacent(color_board, color_board, direction);
}

Board adj_friendly(const GameState& gs, const Color for_color)
{
    Board res;
    const Board& color_board = gs.get_color_board_const(for_color);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        res |= is_adjacent(color_board, color_board, direction);
    return res;
}

Board frozen_pieces(const GameState& gs, const Color for_color)
{
    return adj_enemy_gt(gs, for_color) & ~adj_friendly(gs, for_color);
}

Board mobile_pieces(const GameState& gs, const Color for_color)
{
    return adj_empty(gs, for_color) & ~frozen_pieces(gs, for_color);
}

Board adj_enemy_le(const GameState& gs, const Color for_color)
{
    Board adj_le;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_le |= adj_enemy_le(gs, for_color, direction);
    return adj_le;
}

Board adj_enemy_lt(const GameState& gs, const Color for_color)
{
    Board adj_lt;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_lt |= adj_enemy_lt(gs, for_color, direction);
    return adj_lt;
}

Board adj_enemy_gt(const GameState& gs, const Color for_color)
{
    Board adj_gt;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_gt |= adj_enemy_gt(gs, for_color, direction);
    return adj_gt;
}

Board adj_enemy_ge(const GameState& gs, const Color for_color)
{
    Board adj_ge;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_ge |= adj_enemy_ge(gs, for_color, direction);
    return adj_ge;
}

Step step_from_gs(const GameState& gs, const unsigned int idx, const unsigned int direction)
{
    int c, p;
    gs.color_and_piece_at(idx, &c, &p);
    return Step(c, p, idx, direction);
}
