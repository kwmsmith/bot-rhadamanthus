#ifndef BOARD_H_
#define BOARD_H_

#include <inttypes.h>
#include <string>
#include <vector>
// #include <iostream>
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

enum Action {NORTH, SOUTH, EAST, WEST, ADD, CAPTURE};

unsigned int num_directions();

int direction_from_char(const char ch);

struct Board {

    public:

        Board() : _board(0) {}

        explicit Board(uint64_t board) : _board(board) {}

        explicit Board(const std::vector<uint64_t>& vec) {
            for(std::vector<uint64_t>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
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

        static Board file_n(unsigned int n)
        {
            assert(n >= 1 && n <= 8);
            Board b;
            for(int i=n-1; i < 64; i+=8)
                b.add(i);
            return b;
        }

        static Board rank_n(unsigned int n)
        {
            assert(n >= 1 && n <= 8);
            Board b;
            for(unsigned int i=(n-1)*8; i < n * 8; i++)
                b.add(i);
            return b;
        }

        /**
         * String is in order a1-a8 b1-b8 ... h1-h8, i.e. indices 0..63.
         */
        std::string to_string() const {
            std::string s;
            for(unsigned int i=0; i < 64; ++i) {
                s += (this->contains(i) ? "1" : "0");
            }
            return s;
        }

        void add(uint64_t v) 
        {
            assert(v >= 0 && v < 64);
            _board |= U64_ONE<<v;
        }

        void add(uint64_t v0, uint64_t v1)
        {
            add(v0);
            add(v1);
        }

        void add(uint64_t v0, uint64_t v1, uint64_t v2)
        {
            add(v0);
            add(v1);
            add(v2);
        }

        void add(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3)
        {
            add(v0);
            add(v1);
            add(v2);
            add(v3);
        }

        void add(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4)
        {
            add(v0);
            add(v1);
            add(v2);
            add(v3);
            add(v4);
        }

        void add_file_rank(char file, int rank) {
            assert(file >= 'A' && file <= 'H');
            assert(rank >= 1 && rank <= 8);
            add((rank-1)*8 + (file - 'A'));
        }

        void clear() {
            _board = 0;
        }

        void remove(uint64_t v)
        {
            assert(v >= 0 && v < 64);
            _board &= ~(U64_ONE<<v);
        }

        bool contains(uint64_t v) const 
        { 
            assert(v >= 0 && v < 64);
            return _board & U64_ONE<<v;
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

#endif
