#ifndef TRANSPOSITION_H__
#define TRANSPOSITION_H__

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
            _hash ^= swap_color_;
        }

    private:
        static const uint64_t *get_zobrist_array();
        static const uint64_t swap_color_ = 0x271828DEADBEEFULL;
        uint64_t _hash;
};

struct TTNode
{
    public:
        TTNode()
            : _zobrist(), _depth(0), _score(0) {}

        TTNode(ZobristHash z, uint32_t d, float s)
            : _zobrist(z), _depth(d), _score(s) {}

        ZobristHash _zobrist;
        uint32_t _depth;
        float _score;
        // SType type;
        // Step beststep;
        // bool aged;
};

class TranspositionTable
{
    public:

        TranspositionTable(uint64_t size_MB) 
            : _store(new std::vector<TTNode>), _size_MB(size_MB), _hits(0), _miss(0), _collisions(0) 
        {
            uint64_t nelt = (1024 * 1024 * size_MB) / sizeof(TTNode); 
            nelt = nelt < 1 ? 1 : nelt;
            std::cout << "number of elements: " << nelt << '\n';
            std::cout << "memory used: " << nelt * sizeof(TTNode) / (1024 * 1024) << '\n';
            _store->resize(nelt);
        }

    private:
        boost::scoped_ptr<std::vector<TTNode> > _store;
        uint64_t _size_MB;
        uint64_t _hits, _miss, _collisions; 

};

#endif
