#include "transposition.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

void initialize_zobrist_array(const int size, uint64_t *arr)
{
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
                if (arr[j] == rnd) {
                    collision = true;
                }
            }
            if(collision)
                continue;
        } while(collision);
        arr[i] = uniform(gen);
    }
}
