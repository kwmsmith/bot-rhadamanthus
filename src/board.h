#include <inttypes.h>
#include <iostream>

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
 *      6 | 40                   47 | 6
 *      5 | 32                   39 | 5
 *      4 | 24                   31 | 4
 *      3 | 16                   23 | 3
 *      2 | 08                   15 | 2
 *      1 | 00 01 02 03 04 05 06 07 | 1
 *        +-------------------------+
 *           a  b  c  d  e  f  g  h
 * 
 * --------------------------------------------------------------------------*/

struct Board {

    public:

        Board() : _board(0) {}

        Board(uint64_t board) : _board(board) {}

        Board(const Board &b) 
        { 
            std::cout << "Board copying " << std::endl;
            _board = b._board;
        }

        Board& operator=(const Board &b)
        {
            if (this != &b) {
                std::cout << "Board assignment " << b._board << std::endl;
                _board = b._board;
            }
            return *this;
        }

        void add(uint64_t v) 
        {
            assert(v >= 0 && v < 64);
            _board |= U64_ONE<<v;
        }

        bool contains(uint64_t v) const 
        { 
            assert(v >= 0 && v < 64);
            return _board & U64_ONE<<v;
        }

        void remove(uint64_t v)
        {
            assert(v >= 0 && v < 64);
            _board &= ~(U64_ONE<<v);
        }

        void update(const uint64_t * const vals, unsigned int n)
        {
            for(unsigned int i = 0; i < n; i++) {
                add(vals[i]);
            }
        }

        Board mask(Board m) const {
            return *this & m;
        }

        Board operator|(Board other) const {
            return Board(_board | other._board);
        }

        Board operator&(Board other) const {
            return Board(_board & other._board);
        }

        Board operator^(Board other) const {
            return Board(_board ^ other._board);
        }

        Board operator~() const {
            return Board(~_board);
        }

        uint64_t _board;
    private:

        const static uint64_t U64_ONE = 1;
};
