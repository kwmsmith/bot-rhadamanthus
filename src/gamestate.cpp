#include "gamestate.h"
#include <vector>
#include <cstdio>

#include "boost/algorithm/string.hpp"

using namespace boost;

uint8_t flip_row(uint8_t psn)
{
    uint8_t row = psn / 8;
    uint8_t col = psn % 8;
    uint8_t flip_row = 7 - row;
    return flip_row * 8 + col;
}

void take_step_and_capture(const Step& step, GameState *gs)
{
    assert(step.is_motion());
    gs->move_piece(step);
    Step capture_step;
    if (possible_capture_from_motion(*gs, step)) {
        capture_from_motion(step, gs);
    }
}

void apply_delta_and_capture(const Delta& dd, GameState *gs)
{
    take_step_and_capture(dd.first(), gs);
    if (dd.size() == 2)
        take_step_and_capture(dd.second(), gs);
}


bool gamestate_from_oneline(const std::string& ss, GameState *gs)
{
    int idx = 0;
    char ch;
    do {
        ch = ss[idx++];
    } while(ch == ' ');

    if (ch == 'w' || ch == 'g') {
        gs->set_color(W);
    } else if (ch == 'b' || ch == 's') {
        gs->set_color(B);
    } else {
        return false;
    }
    
    do {
        ch = ss[idx++];
    } while(ch == ' ');

    if (ch != '[') {
        return false;
    }
    
    for(int i=idx; i<idx+64; ++i) {
       int piece_color, this_piece;
       char ch = ss[i];
       if (ch == ' ' || ch == 'x' || ch == 'X' || ch == '.')
           continue;
       this_piece = piece_from_char(ch);
       if (this_piece == kInvalidPiece)
           return false;
       piece_color = color_from_char(ch);
       gs->add_piece_at(piece_color, this_piece, flip_row(idx));
    }
    return true;
}

bool gamestate_from_input(const std::string& ss, GameState *gs)
{
    gs->clear(); // error sentinel.
    std::vector<std::string> lines;
    split(lines, ss, is_any_of("\n"), token_compress_on);
    for (unsigned int i=0; i < lines.size(); ++i) {
        lines[i] = trim_copy(lines[i]);
    }
    // get color to move.
    std::vector<std::string> tokens;
    split(tokens, lines[0], is_any_of(" "), token_compress_on);
    to_lower(tokens[0]);
    const char color = tokens[0][tokens[0].size() - 1];
    if (color == 'w' || color == 'g') {
        gs->set_color(W);
    } else if (color == 'b' || color == 's') {
        gs->set_color(B);
    } else {
        return false;
    }
    
    // parse game position
    
    const int start_line = 2;
    for (unsigned int line_idx=start_line; line_idx < start_line+8; ++line_idx) {
        tokens.clear();
        // After splitting on '|', tokens[0] is the line number.  tokens[1] is
        // the board contents.
        split(tokens, lines[line_idx], is_any_of("|"), token_compress_on);
        
        // row_idx: labels the row we're in such that row_idx == 0 is gold's
        // territory; row_idx == 7 is silver's.
        uint8_t row_idx = 7 - (line_idx - start_line);
        int piece_color;
        int this_piece;
        uint8_t idx;
        for (uint8_t col_idx=0; col_idx < tokens[1].size() - 1; col_idx += 2) {
            const char piece_or_empty = tokens[1][col_idx+1];
            if (piece_or_empty == ' ' || piece_or_empty == 'x' ||
                piece_or_empty == 'X' || piece_or_empty == '.') {
                continue;
            }
            this_piece = piece_from_char(piece_or_empty);
            if (this_piece == kInvalidPiece)
                return false;
            piece_color = color_from_char(piece_or_empty);
            idx = row_idx * 8 + col_idx / 2;
            gs->add_piece_at(piece_color, this_piece, idx);
        }
    }
    return true;
}

std::string GameState::to_std_string(const char empty) const
{
    const std::string &oneline = to_oneline_string(empty);
    std::string s(" +-----------------+\n");
    int rev_row = 0;
    char buf[10];
    for(int row=0; row < 8; row++) {
        rev_row = 7 - row;
        sprintf(buf, "%d", rev_row + 1);
        s += buf;
        s += '|';
        s += ' ';
        for(int col=0; col < 8; col++) {
            s += oneline[rev_row * 8 + col];
            s += ' ';
        }
        s += "|\n";
    }
    s += " +-----------------+\n";
    s += "   a b c d e f g h\n";
    return s;
}

std::string GameState::to_oneline_string(const char empty) const 
{
    std::string s(64, empty);
    
    // put x's on the trap squares
    s[flip_row(18)] = 'x'; s[flip_row(21)] = 'x';
    s[flip_row(42)] = 'x'; s[flip_row(45)] = 'x';

    const int colors[] = {W, B};
    const char piece_char[] = { 'R', 'C', 'D', 'H', 'M', 'E'};
    const int lower_offset = 'a' - 'A';
    for(int i=0; i<2; i++) {
        for(int p=R; p < nPieces; p++) {
            Board b = _color[colors[i]] & _pieces[p];
            while(!b.is_empty()) {
                uint8_t psn = b.idx_and_reset();
                s[flip_row(psn)] = i==W ? piece_char[p] : (piece_char[p]+lower_offset);
            }
        }
    }
    return '[' + s + ']';
}

bool GameState::add_piece_at(const Step &s)
{
    return add_piece_at(s.get_color(), s.get_piece(), s.get_position());
}

bool GameState::add_piece_at(const uint8_t c, const uint8_t piece, const uint8_t idx)
{
    assert(c == W || c == B);
    assert(piece <= E);
    assert(idx < 64);
    if (get_all_const().contains(idx)) {
        return false; // position already occupied.
    }
    add_piece_at_fast(c, piece, idx);
    assert(_pieces[piece].contains(idx));
    assert(get_color_board(c).contains(idx));
    return true;
}

bool GameState::remove_piece_at(const uint8_t c, const uint8_t piece, const uint8_t idx)
{
    if (!(_pieces[piece] & get_color_board(c)).contains(idx)) {
        return false; // idx does not contain the right piece.
    }
    remove_piece_at_fast(c, piece, idx);
    assert(!(_pieces[piece] & get_color_board(c)).contains(idx));
    return true;
}

bool GameState::move_piece(const Step &s)
{
    assert(s.is_motion());
    const uint8_t c = s.get_color(), p = s.get_piece(), pos = s.get_position(), finish = s.get_finish();
    return move_piece(c, p, pos, finish);
}

bool GameState::move_piece(const uint8_t c, const uint8_t piece, const uint8_t from, const uint8_t to)
{
    // TODO: Test for move off of board?
    
    assert(_stepsleft >= 1);

    // now we can move the piece.
    remove_piece_at(c, piece, from);
    add_piece_at(c, piece, to);
    _stepsleft--;
    return true;
}

bool GameState::is_empty() const
{
    Board b(get_all_const());
    for(int i=R; i < nPieces; ++i)
        b |= _pieces[i];
    return b.is_empty();
}


void generate_pushes(const GameState& gs, std::vector<Delta> *pushes)
{
    const uint8_t pushing_color = gs.get_color();
    const uint8_t pushed_color = other_color(gs.get_color());
    const Board& pushing_mobile = ~frozen_pieces(gs, gs.get_color());

    Board pushed_with_adj_empty[4];
    
    // The body of generate_pushes() takes the perspective of the pushed piece.  
    // dir_pushed_from is the direction from which the pushing piece comes from.
    // dir_pushed is the direction the pushed piece is pushed to.
    
    for (unsigned int dir = NORTH; dir < num_directions(); ++dir) {
        pushed_with_adj_empty[dir] = adj_empty(gs, pushed_color, dir);
    }
    
    for (unsigned int dir_pushed_from = NORTH; dir_pushed_from < num_directions(); ++dir_pushed_from) {
        
        // the mobile pushing pieces with an adjacent weaker piece.
        // We use the opp_dir(dir_pushed_from) here since we are talking about the pushing piece.
        const Board& pushing_pieces_with_adj_lt = 
            adj_enemy_lt(gs, pushing_color, opp_dir(dir_pushed_from)) & pushing_mobile;
        
        for (unsigned int dir_pushed = NORTH; dir_pushed < num_directions(); ++dir_pushed) {
            if (dir_pushed_from == dir_pushed) continue;
            
            // TODO: Try and make symmetric with generate_pulls() -- simplify inner loop here...
            Board pushing_pieces = 
                is_adjacent(pushing_pieces_with_adj_lt, pushed_with_adj_empty[dir_pushed],
                        opp_dir(dir_pushed_from));
                
            Board pushed_pieces = 
                is_adjacent(pushed_with_adj_empty[dir_pushed], pushing_pieces_with_adj_lt, dir_pushed_from);
            
            while(!pushed_pieces.is_empty()) {
                assert(!pushing_pieces.is_empty());
                uint8_t pushed_idx = pushed_pieces.idx_and_reset();
                uint8_t pusher_idx = pushing_pieces.idx_and_reset();
                assert(gs.get_all_const().contains(pushed_idx));
                assert(gs.get_all_const().contains(pusher_idx));
                pushes->push_back(Delta(step_from_gs(gs, pushed_idx, dir_pushed),
                                        step_from_gs(gs, pusher_idx, opp_dir(dir_pushed_from))));
            }
        }
    }
}

void generate_pulls(const GameState& gs, std::vector<Delta> *pulls)
{
    const uint8_t pulling_color = gs.get_color();
    const Board& pulling_mobile = ~frozen_pieces(gs, gs.get_color());
    
    Board pulling_with_adj_empty[4];
    
    // The body of generate_pushes() takes the perspective of the pulling piece.
    
    for (unsigned int dir = NORTH; dir < num_directions(); ++dir) {
        pulling_with_adj_empty[dir] = adj_empty(gs, pulling_color, dir);
    }
    
    for (unsigned int dir_pulled_piece = NORTH; dir_pulled_piece < num_directions(); ++dir_pulled_piece) {
        
        // the mobile pulling pieces with an adjacent weaker piece.
        // We use the opp_dir(dir_pulled_piece) here since we are talking about the pushing piece.
        const Board& pulling_pieces_with_adj_lt = 
            adj_enemy_lt(gs, pulling_color, dir_pulled_piece) & pulling_mobile;
        
        for (unsigned int dir_pulling_piece = NORTH; dir_pulling_piece < num_directions(); ++dir_pulling_piece) {
            if (dir_pulled_piece == dir_pulling_piece) continue;
            
            Board pulling_pieces = pulling_pieces_with_adj_lt & pulling_with_adj_empty[dir_pulling_piece];
            
            Board pulled_pieces = pulling_pieces.move(dir_pulled_piece);
            
            while(!pulled_pieces.is_empty()) {
                assert(!pulling_pieces.is_empty());
                uint8_t pulled_idx = pulled_pieces.idx_and_reset();
                uint8_t pulling_idx = pulling_pieces.idx_and_reset();
                assert(gs.get_all_const().contains(pulled_idx));
                assert(gs.get_all_const().contains(pulling_idx));
                pulls->push_back(Delta(step_from_gs(gs, pulling_idx, dir_pulling_piece),
                                       step_from_gs(gs, pulled_idx, opp_dir(dir_pulled_piece))));
            }
        }
    }
}

void generate_steps(const GameState& gs, std::vector<Delta> *steps)
{
    const Board& not_frozen = ~frozen_pieces(gs, gs.get_color());

    for (unsigned int dir_empty = NORTH; dir_empty < num_directions(); ++dir_empty) {

        Board pieces_with_step = adj_step(gs, gs.get_color(), dir_empty) & not_frozen;
        
        assert((pieces_with_step & gs.get_color_board(gs.get_color())) == pieces_with_step);

        while(!pieces_with_step.is_empty()) {
            uint8_t mobile_idx = pieces_with_step.idx_and_reset();
            assert(gs.get_all_const().contains(mobile_idx));
            steps->push_back(Delta(step_from_gs(gs, mobile_idx, dir_empty)));
        }
    }
}

bool possible_capture_from_motion(const GameState &gs, const Step& step_taken)
{
    const unsigned char finish = step_taken.get_finish();
    const bool moved_onto_capture = Board::is_capture_idx(finish);
    if (moved_onto_capture)
        return true;
    const bool started_adj_to_capture = Board::is_adj_capture_squares(step_taken.get_position());
    if (started_adj_to_capture)
        return true;
    return false;
}

void capture_from_motion(const Step& step_taken, GameState *gs)
{
    const uint8_t color = step_taken.get_color();
    Board capturable = 
        gs->get_color_board(color) & ~adj_friendly(*gs, color) & Board::capture_squares();
    if (capturable.is_empty())
        return;
    uint8_t capture_idx = capturable.idx_and_reset();
    assert(capturable.is_empty());
    int8_t c, p;
    gs->color_and_piece_at(capture_idx, &c, &p);
    gs->remove_piece_at(c, p, capture_idx);
}

bool detect_capture_from_motion(const GameState& gs, const Step& step_taken, Step *capture)
{
    if (!possible_capture_from_motion(gs, step_taken))
        return false;
    
    const unsigned char color = step_taken.get_color();
    const Board capturable = gs.get_color_board(color) & ~adj_friendly(gs, color);
    const char capture_idxs[] = {18, 21, 42, 45};
    for(int i=0; i < 4; ++i) {
        if (capturable.contains(capture_idxs[i])) {
            // we have a capture!
            int8_t c, p;
            gs.color_and_piece_at(capture_idxs[i], &c, &p);
            // assert(c == color);
            *capture = Step(color, p, capture_idxs[i], CAPTURE);
            return true;
        }
    }
    return false;
}

Board adj_enemy_gt(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    Board adj_gt, enemy_gt, these_pieces;
    const Board& color_board = gs.get_color_board(for_color);
    const Board& enemy_color = gs.get_color_board(other_color(for_color));
    for(int p = E; p >= R; --p) {
        these_pieces = gs.get_piece_board(p) & color_board;
        adj_gt |= is_adjacent(these_pieces, enemy_gt, direction);
        enemy_gt |= gs.get_piece_board(p) & enemy_color;
    }
    return adj_gt;
}

Board adj_enemy_ge(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    Board adj_ge, enemy_ge, these_pieces;
    const Board& color_board = gs.get_color_board(for_color);
    const Board& enemy_color = gs.get_color_board(other_color(for_color));
    for(int p = E; p >= R; --p) {
        enemy_ge |= gs.get_piece_board(p) & enemy_color;
        these_pieces = gs.get_piece_board(p) & color_board;
        adj_ge |= is_adjacent(these_pieces, enemy_ge, direction);
    }
    return adj_ge;
}

Board adj_enemy_lt(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    Board adj_lt, enemy_lt, these_pieces;
    const Board& color_board = gs.get_color_board(for_color);
    const Board& enemy_color = gs.get_color_board(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        these_pieces = gs.get_piece_board(p) & color_board;
        adj_lt |= is_adjacent(these_pieces, enemy_lt, direction);
        enemy_lt |= gs.get_piece_board(p) & enemy_color;
    }
    return adj_lt;
}

Board adj_enemy_le(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    Board adj_le, enemy_le, these_pieces;
    const Board& color_board = gs.get_color_board(for_color);
    const Board& enemy_color = gs.get_color_board(other_color(for_color));
    for(int p = R; p < nPieces; ++p) {
        enemy_le |= gs.get_piece_board(p) & enemy_color;
        these_pieces = gs.get_piece_board(p) & color_board;
        adj_le |= is_adjacent(these_pieces, enemy_le, direction);
    }
    return adj_le;
}

Board adj_step(const GameState& gs, const uint8_t for_color)
{
    Board adj_step_;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_step_ |= adj_step(gs, for_color, direction);
    return adj_step_;
}

Board adj_step(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    const Board& color_board = gs.get_color_board(for_color);
    const Board empties = ~gs.get_all_const();
    const Board adj = is_adjacent(color_board, empties, direction);
    if (direction == SOUTH && for_color == W) {
        Board rabbit_mask = gs.get_piece_board(R) & color_board;
        return adj & ~rabbit_mask;
    } else if (direction == NORTH && for_color == B) {
        Board rabbit_mask = gs.get_piece_board(R) & color_board;
        return adj & ~rabbit_mask;
    } else {
        return adj;
    }
}

Board adj_empty(const GameState& gs, const uint8_t for_color)
{
    Board adj_mt;
    const Board& color_board = gs.get_color_board(for_color);
    const Board empties = ~gs.get_all_const();
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_mt |= is_adjacent(color_board, empties, direction);
    return adj_mt;
}

Board adj_empty(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    const Board& color_board = gs.get_color_board(for_color);
    const Board empties = ~gs.get_all_const();
    return is_adjacent(color_board, empties, direction);
}

Board adj_friendly(const GameState& gs, const uint8_t for_color, const unsigned int direction)
{
    const Board& color_board = gs.get_color_board(for_color);
    return is_adjacent(color_board, color_board, direction);
}

Board adj_friendly(const GameState& gs, const uint8_t for_color)
{
    Board res;
    const Board& color_board = gs.get_color_board(for_color);
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        res |= is_adjacent(color_board, color_board, direction);
    return res;
}

Board frozen_pieces(const GameState& gs, const uint8_t for_color)
{
    return adj_enemy_gt(gs, for_color) & ~adj_friendly(gs, for_color);
}

Board mobile_pieces(const GameState& gs, const uint8_t for_color)
{
    return adj_step(gs, for_color) & ~frozen_pieces(gs, for_color);
}

Board adj_enemy_le(const GameState& gs, const uint8_t for_color)
{
    Board adj_le;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_le |= adj_enemy_le(gs, for_color, direction);
    return adj_le;
}

Board adj_enemy_lt(const GameState& gs, const uint8_t for_color)
{
    Board adj_lt;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_lt |= adj_enemy_lt(gs, for_color, direction);
    return adj_lt;
}

Board adj_enemy_gt(const GameState& gs, const uint8_t for_color)
{
    Board adj_gt;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_gt |= adj_enemy_gt(gs, for_color, direction);
    return adj_gt;
}

Board adj_enemy_ge(const GameState& gs, const uint8_t for_color)
{
    Board adj_ge;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_ge |= adj_enemy_ge(gs, for_color, direction);
    return adj_ge;
}

Step step_from_gs(const GameState& gs, const uint8_t idx, const unsigned int direction)
{
    int8_t c, p;
    gs.color_and_piece_at(idx, &c, &p);
    return Step(c, p, idx, direction);
}

void get_num_pieces_array(const GameState& gs, const uint8_t for_color, uint8_t *num_pieces)
{
    const Board& color_board = gs.get_color_board(for_color);
    for (uint8_t i=0; i < nPieces; i++) {
        num_pieces[i] = (gs.get_piece_board(i) & color_board).count();
    }
}

// candidates for removal.

// void has_adjacent_empty_directional(const GameState& gs, const Color c, std::vector<Board> *boards)
// {
    // const Board empties = ~gs.get_all_const();
    // const Board& color_board = gs.get_color_board(c);
    // for(unsigned int direction = NORTH; direction < num_directions(); ++direction) {
        // boards->push_back(is_adjacent(color_board, empties, direction));
    // }
// }

// void mobile_pieces_directional(const GameState& gs, const Color c, std::vector<Board> *boards)
// {
    // Board not_frozen = ~frozen_pieces(gs, c);
    // has_adjacent_empty_directional(gs, c, boards);
    // for(unsigned int i=0; i < num_directions(); ++i) {
        // boards->at(i) &= not_frozen;
    // }
// }
