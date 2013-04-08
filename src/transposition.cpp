#include "transposition.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

uint64_t *ZobristHash::get_zobrist_array()
{
    static uint64_t zarr_[2][nPieces][64];
    return &zarr_[0][0][0];
}

void ZobristHash::init_zobrist_array()
{
    static bool initialized = false;
    if (initialized) {
        return;
    }
    const int size = 2 * nPieces * 64;
    uint64_t *zarr = get_zobrist_array();
    
    boost::random::mt19937_64 gen(2718281828459045ULL);
    boost::random::uniform_int_distribution<uint64_t> uniform;
    
    uint64_t rnd;
    bool collision = false;
    
    for(int i=0; i < size; ++i) {
        do {
            collision = false;
            rnd = uniform(gen);
            if (rnd == 0)
                collision = true;
            for (int j=0; j < i; ++j) {
                if (zarr[j] == rnd) {
                    collision = true;
                }
            }
            if(collision)
                continue;
        } while(collision);
        zarr[i] = uniform(gen);
    }
    initialized = true;
}
