#ifndef BOARD_H_
#define BOARD_H_

#include <inttypes.h>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

/* ----------------------------------------------------------------------------
 *  Implementation details:
 * 
 *  The most sig. bit of a bitboard corresponds to square h8 of an Arimaa
 *  board.  The least sig. bit of a bitboard corresponds to squard a1 of an
 *  Arimaa board.
 * 
 *  North corresponds to adding 8 to an index (left shifting a bb by 8).
 *  South corresponds to subtracting 8 from an index(right shifting a bb by 8).
 *  East corresponds to adding 1 to an index (right shifting a bb by 1).
 *  West corresponds to subtracting 1 from an index (left shifting a bb by 1).
 * 
 *           a  b  c  d  e  f  g  h
 *        +-------------------------+
 *      8 | 56 57 58 59 60 61 62 63 | 8
 *      7 | 48                   55 | 7
 *      6 | 40    42       45    47 | 6
 *      5 | 32                   39 | 5
 *      4 | 24                   31 | 4
 *      3 | 16    18       21    23 | 3
 *      2 | 08                   15 | 2
 *      1 | 00 01 02 03 04 05 06 07 | 1
 *        +-------------------------+
 *           a  b  c  d  e  f  g  h
 * 
 * --------------------------------------------------------------------------*/

enum Piece { R, C, D, H, M, E, nPieces };

const static int kInvalidPiece = -1;

enum Color { W, B };

inline Color other_color(Color c) 
{
    return c == W ? B : W;
}

enum Action {NORTH, SOUTH, EAST, WEST, ADD, CAPTURE};

/**
 * Takes a display integer (0 -> square h1, 63 -> square a8), and converts it
 * to its internal index.
 */
inline int internal_idx_from_display(uint8_t d)
{
    int row =  - d / 8 + 7;
    int col = d % 8;
    return 8 * row + col;
}

inline int color_from_char(char ch)
{
    switch(ch) {
        case 'R':
        case 'C':
        case 'D':
        case 'H':
        case 'M':
        case 'E':
            return W;
        case 'r':
        case 'c':
        case 'd':
        case 'h':
        case 'm':
        case 'e':
            return B;
        default:
            return kInvalidPiece;
    }
}

inline int piece_from_char(char ch)
{
    switch(ch) {
        case 'R':
        case 'r':
            return R;
        case 'C':
        case 'c':
            return C;
        case 'D':
        case 'd':
            return D;
        case 'H':
        case 'h':
            return H;
        case 'M':
        case 'm':
            return M;
        case 'E':
        case 'e':
            return E;
        default:
            return kInvalidPiece;
    }
}


unsigned int num_directions();
int direction_from_char(const char ch);
unsigned int opp_dir(unsigned int);

// inline uint8_t next_psn(Board &b)
// {
    // register Board next_psn = b & -b;
// }

class Board {

    public:

        Board() : _board(0) {}

        explicit Board(uint64_t board) : _board(board) {}

        explicit Board(const std::vector<uint8_t>& vec) {
            for(std::vector<uint8_t>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
                add(*it);
            }
        }

        /**
         * String is in order a1-a8 b1-b8 ... h1-h8, i.e. indices 0..63.
         */
        explicit Board(const std::string& s) :_board(0) {
            size_t lim = std::min(s.length(), (size_t)64);
            for(unsigned int i=0; i < lim; ++i) {
                if(s[i] == '0')
                    continue;
                _board |= (U64_ONE<<i);
            }
        }

        Board(const Board &b) 
        { 
            _board = b._board;
        }

        static Board file_n(const uint8_t n)
        {
            assert(n >= 1 && n <= 8);
            Board b;
            for(uint8_t i=n-1; i < 64; i+=8)
                b.add(i);
            return b;
        }

        static bool is_capture_idx(const char idx) {
            return (idx == 18 || idx == 21 || idx == 42 || idx == 45);
        }
        
        static bool is_adj_capture_squares(uint8_t idx) {
            const static Board adj_captures(0x245a24245a2400ULL);
            return adj_captures.contains(idx);
        }

        static Board capture_squares()
        {
            const static Board _captures(0x240000240000ULL);
            return _captures;
        }

        static Board rank_n(const uint8_t n)
        {
            assert(n >= 1 && n <= 8);
            Board b;
            for(uint8_t i=(n-1)*8; i < n * 8; i++)
                b.add(i);
            return b;
        }

        /**
         * String is in order a1-a8 b1-b8 ... h1-h8, i.e. indices 0..63.
         */
        std::string to_string() const {
            std::string s;
            for(uint8_t i=0; i < 64; ++i) {
                s += (this->contains(i) ? "1" : "0");
            }
            return s;
        }

        Board& add(uint8_t v) 
        {
            assert(v < 64);
            _board |= U64_ONE<<v;
            return *this;
        }

        Board& add(uint8_t v0, uint8_t v1)
        {
            add(v0);
            return add(v1);
        }

        Board& add(uint8_t v0, uint8_t v1, uint8_t v2)
        {
            add(v0);
            add(v1);
            return add(v2);
        }

        Board& add(uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3)
        {
            add(v0);
            add(v1);
            add(v2);
            return add(v3);
        }

        Board& add(uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4)
        {
            add(v0);
            add(v1);
            add(v2);
            add(v3);
            return add(v4);
        }

        Board& add_file_rank(const uint8_t file, const uint8_t rank) {
            assert(file >= 'A' && file <= 'H');
            assert(rank >= 1 && rank <= 8);
            return add((rank-1)*8 + (file - 'A'));
        }

        void clear() {
            _board = 0;
        }
        
        bool any() const {
            return _board;
        }
        
        bool is_empty() const {
            return _board == 0;
        }

        Board& remove(uint8_t v)
        {
            assert(v < 64);
            _board &= ~(U64_ONE<<v);
            return *this;
        }

        bool contains(uint8_t v) const 
        { 
            assert(v < 64);
            return _board & (U64_ONE<<v);
        }
        
        bool contains(const uint8_t file, const uint8_t rank) const {
            assert(file >= 'A' && file <= 'H');
            assert(rank >= 1 && rank <= 8);
            return contains((rank - 1) * 8 + (file - 'A'));
        }
        
        uint8_t idx_and_reset() {
            uint8_t idx = _board ? __builtin_ctzl(_board) : 64;
            _board ^= (_board & -_board);
            return idx;
        }
        
        const uint8_t count() const {
            // TODO: Try other bit counting methods to speed this one up.
            uint64_t tmp = _board, ret = 0;
            for(ret=0; tmp; ret++) {
                tmp &= tmp - 1;
            }
            return ret;
        }
        
        Board mask(Board m) const {
            return *this & m;
        }

        Board& operator=(const Board &b)
        {
            if (this != &b) {
                _board = b._board;
            }
            return *this;
        }

        Board operator|(const Board& other) const {
            return Board(_board | other._board);
        }

        void operator|=(const Board& other) {
            _board |= other._board;
        }

        Board operator&(const Board& other) const {
            return Board(_board & other._board);
        }

        void operator&=(const Board& other) {
            _board &= other._board;
        }

        Board operator^(const Board& other) const {
            return Board(_board ^ other._board);
        }

        void operator^=(const Board& other) {
            _board ^= other._board;
        }

        Board operator~() const {
            return Board(~_board);
        }

        Board flip() const {
            return Board(~_board);
        }

        bool operator==(const Board& other) const {
            return _board == other._board;
        }

        bool operator==(uint64_t o) const {
            return _board == o;
        }
        
        Board move_opp(const uint8_t direction) const {
            const static Board not_file_8 = file_n(8).flip();
            const static Board not_file_1 = file_n(1).flip();
            switch(direction) {
                case NORTH:
                    return Board(_board >> 8); // moves South.
                    break;
                case SOUTH:
                    return Board(_board << 8); // moves North.
                    break;
                case EAST:
                    return Board((_board & not_file_1._board) >> 1); // moves West.
                    break;
                case WEST:
                    return Board((_board & not_file_8._board) << 1); // moves East.
                    break;
            }
            assert(0);
        }

        Board move(int direction) const {
            const static Board not_file_8 = file_n(8).flip();
            const static Board not_file_1 = file_n(1).flip();
            switch(direction) {
                case NORTH:
                    return Board(_board << 8);
                    break;
                case SOUTH:
                    return Board(_board >> 8);
                    break;
                case EAST:
                    return Board((_board & not_file_8._board) << 1);
                    break;
                case WEST:
                    return Board((_board & not_file_1._board) >> 1);
                    break;
            }
            assert(0);
        }

    private:

        uint64_t _board;
        const static uint64_t U64_ONE = 1;
};

inline Board is_adjacent(const Board& stationary, const Board& moved, const uint8_t direction)
{
    return moved.move_opp(direction) & stationary;
}

#endif
