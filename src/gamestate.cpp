#include "gamestate.h"
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
    assert(gs->get_hash());
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
        Board b = _color[W] & _pieces[p];
        while(!b.is_empty()) {
            uint8_t psn = b.idx_and_reset();
            s[psn] = white_char[p];
        }
    }
    for(int p=R; p < nPieces; p++) {
        Board b = _color[B] & _pieces[p];
        while(!b.is_empty()) {
            uint8_t psn = b.idx_and_reset();
            s[psn] = black_char[p];
        }
    }
    return s;
}

bool GameState::take_step(const Step &s)
{
    if (!s.is_valid())
        return false;

    const int c = s.get_color(), p = s.get_piece(), pos = s.get_position(), finish = s.get_finish();

    assert(c == W || c == B);

    switch(s.get_action()) {
        case NORTH:
            return move_piece(c, p, pos, finish);
        case SOUTH:
            return move_piece(c, p, pos, finish);
        case EAST:
            return move_piece(c, p, pos, finish);
        case WEST:
            return move_piece(c, p, pos, finish);
        case CAPTURE:
            return remove_piece_at(c, p, pos);
        case ADD:
            return add_piece_at(c, p, pos);
    }
    assert(0); // should never get here.
    return false;
}

bool GameState::add_piece_at(const int c, const int piece, const uint8_t idx)
{
    assert(c == W || c == B);
    assert(piece >= R && piece <= E);
    assert(idx < 64);
    if (get_all_const().contains(idx)) {
        return false; // position already occupied.
    }
    add_piece_at_fast(c, piece, idx);
    assert(_pieces[piece].contains(idx));
    assert(get_color_board_const(c).contains(idx));
    return true;
}

bool GameState::remove_piece_at(const int c, const int piece, const uint8_t idx)
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
bool GameState::move_piece(const int c, const int piece, const uint8_t from, const uint8_t to)
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
    return b.is_empty();
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

void generate_pushes(const GameState& gs, const Color for_color, std::vector<Step> *pushes)
{
    const Color& pushing_color = for_color;
    const Color& pushed_color = other_color(for_color);
    const Board& pushing_mobile = ~frozen_pieces(gs, for_color);

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
                pushes->push_back(step_from_gs(gs, pushed_idx, dir_pushed));
                pushes->push_back(step_from_gs(gs, pusher_idx, opp_dir(dir_pushed_from)));
            }
        }
    }
}

void generate_pulls(const GameState& gs, const Color for_color, std::vector<Step> *pulls)
{
    const Color& pulling_color = for_color;
    const Board& pulling_mobile = ~frozen_pieces(gs, for_color);
    
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
                pulls->push_back(step_from_gs(gs, pulling_idx, dir_pulling_piece));
                pulls->push_back(step_from_gs(gs, pulled_idx, opp_dir(dir_pulled_piece)));
            }
        }
    }
}

void generate_steps(const GameState& gs, const Color for_color, std::vector<Step> *steps)
{
    const Board& not_frozen = ~frozen_pieces(gs, for_color);

    for (unsigned int dir_empty = NORTH; dir_empty < num_directions(); ++dir_empty) {

        Board pieces_with_step = adj_step(gs, for_color, dir_empty) & not_frozen;
        
        assert((pieces_with_step & gs.get_color_board_const(for_color)) == pieces_with_step);

        while(!pieces_with_step.is_empty()) {
            uint8_t mobile_idx = pieces_with_step.idx_and_reset();
            assert(gs.get_all_const().contains(mobile_idx));
            steps->push_back(step_from_gs(gs, mobile_idx, dir_empty));
        }
    }
}

uint8_t generate_captures(const GameState& gs, std::vector<Step> *captures)
{
    uint8_t ncaptures = 0;
    Board captured = Board::capture_squares() & 
        (gs.get_all_const() & ~(adj_friendly(gs, B) | adj_friendly(gs, W)));
    
    while(!captured.is_empty()) {
        uint8_t captured_idx = captured.idx_and_reset();
        assert(gs.get_all_const().contains(captured_idx));
        captures->push_back(step_from_gs(gs, captured_idx, CAPTURE));
        ncaptures++;
    }
    return ncaptures;
}

bool detect_capture_from_motion(const GameState& gs, const Step& step_taken, Step *capture)
{
    assert(step_taken.is_motion());
    // Common case: move doesn't even land on a capture square.
    const unsigned char finish = step_taken.get_finish();
    const bool started_adj_to_capture = Board::is_adj_capture_squares(step_taken.get_position());
    const bool moved_onto_capture = Board::is_capture_idx(finish);
    if (!moved_onto_capture && !started_adj_to_capture)
        return false;
    
    const unsigned char color = step_taken.get_color();
    const Color color_enum = (color == W ? W : B);
    const Board capturable = gs.get_color_board_const(color) & ~adj_friendly(gs, color_enum);
    const char capture_idxs[] = {18, 21, 42, 45};
    for(int i=0; i < 4; ++i) {
        if (capturable.contains(capture_idxs[i])) {
            // we have a capture!
            int c, p;
            gs.color_and_piece_at(capture_idxs[i], &c, &p);
            assert(c == color);
            *capture = Step(color, p, capture_idxs[i], CAPTURE);
            return true;
        }
    }
    return false;
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
        adj_lt |= is_adjacent(these_pieces, enemy_lt, direction);
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

Board adj_step(const GameState& gs, const Color for_color)
{
    Board adj_step_;
    for(unsigned int direction = NORTH; direction < num_directions(); ++direction)
        adj_step_ |= adj_step(gs, for_color, direction);
    return adj_step_;
}

Board adj_step(const GameState& gs, const Color for_color, const unsigned int direction)
{
    const Board& color_board = gs.get_color_board_const(for_color);
    const Board empties = ~gs.get_all_const();
    const Board adj = is_adjacent(color_board, empties, direction);
    if (direction == SOUTH && for_color == W) {
        Board rabbit_mask = gs.get_piece_board_const(R) & color_board;
        return adj & ~rabbit_mask;
    } else if (direction == NORTH && for_color == B) {
        Board rabbit_mask = gs.get_piece_board_const(R) & color_board;
        return adj & ~rabbit_mask;
    } else {
        return adj;
    }
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
    return adj_step(gs, for_color) & ~frozen_pieces(gs, for_color);
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

Step step_from_gs(const GameState& gs, const uint8_t idx, const unsigned int direction)
{
    int c, p;
    gs.color_and_piece_at(idx, &c, &p);
    return Step(c, p, idx, direction);
}

void get_num_pieces_array(const GameState& gs, const Color for_color, uint8_t *num_pieces)
{
    const Board& color_board = gs.get_color_board_const(for_color);
    for (uint8_t i=0; i < nPieces; i++) {
        num_pieces[i] = (gs.get_piece_board_const(i) & color_board).count();
    }
}
