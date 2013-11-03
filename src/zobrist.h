#ifndef ZOBRIST_H__
#define ZOBRIST_H__

#include <inttypes.h>
#include "board.h"
#include "boost/scoped_ptr.hpp"

class ZobristHash
{
    public:

        ZobristHash() : _hash(0ULL) {};

        void clear() { _hash = 0ULL; };

        const uint64_t get_hash() const { return _hash; };

        void addrm_piece_at(const int color, const int piece, const int idx) {
            const uint64_t *zarr = get_zobrist_array();
            const int zidx = idx + 64 * (piece + nPieces * color);
            _hash ^= zarr[zidx];
        }

        void flip_color() {
            _hash ^= _swap_color;
        }

        bool operator==(const ZobristHash &other) {
            return _hash == other._hash;
        }

    private:
        static const uint64_t *get_zobrist_array();
        static const uint64_t _swap_color = 0x271828DEADBEEFULL;
        uint64_t _hash;
};

#endif
