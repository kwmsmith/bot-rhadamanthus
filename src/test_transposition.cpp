#include "transposition.h"
#include "gtest/gtest.h"

#include <iostream>

TEST(zobrist_hash, hash) {
    uint64_t arr[10];
    initialize_zobrist_array(10, arr);
    for(int i=0; i < 10; ++i) {
        EXPECT_TRUE(arr[i] != 0);
        std::cout << arr[i] << std::endl;
    }
}
