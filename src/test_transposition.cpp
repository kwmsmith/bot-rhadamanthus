#include "transposition.h"
#include "gtest/gtest.h"

#include <iostream>

TEST(ZobristHash, hash) {
  ZobristHash zhash;
  EXPECT_EQ(sizeof(zhash), sizeof(uint64_t));
  EXPECT_EQ(zhash.get_hash(), 0ULL);
  zhash.addrm_piece_at(B, D, 32);
  EXPECT_TRUE(zhash.get_hash() != 0);
  zhash.addrm_piece_at(B, D, 32);
  EXPECT_EQ(zhash.get_hash(), 0);
}

TEST(transposition, TranspositionTable) {
  TranspositionTable tt(512);
}
