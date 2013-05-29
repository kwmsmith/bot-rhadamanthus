#ifndef TRANSPOSITION_H__
#define TRANSPOSITION_H__

#include <inttypes.h>
#include "board.h"

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
            _hash ^= swap_color_;
        }
        
    private:
        static const uint64_t *get_zobrist_array();
        static const uint64_t swap_color_ = 0x271828DEADBEEFULL;
        uint64_t _hash;
};

#endif
