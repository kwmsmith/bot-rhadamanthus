#include <inttypes.h>

struct Board {

    public:

        Board() : _board(0) {}

        Board(uint64_t board) : _board(board) {}

        uint64_t _board;
};
