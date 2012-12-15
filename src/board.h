#include <inttypes.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

struct Board {

    public:

        Board() : _board(0) {}

        Board(uint64_t board) : _board(board) {}

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

        void update(unsigned long *vals, unsigned int n)
        {
            for(unsigned int i = 0; i < n; i++) {
                add(vals[i]);
            }
        }

        uint64_t _board;

        const static uint64_t U64_ONE = 1;
};
