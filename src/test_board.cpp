#include "board.h"
#include "gtest/gtest.h"
#include <climits>
#include <cstdio>
#include <type_traits>

TEST(BoardTest, std_layout) {
  EXPECT_TRUE(std::is_standard_layout<Board>::value);
}

TEST(BoardTest, trivially_copyable) {
  EXPECT_TRUE(std::is_trivially_copyable<Board>::value);
}

TEST(BoardTest, Instantiate) {
  Board b;
  EXPECT_EQ(b, 0ULL);
}

TEST(Direction, order) {
  EXPECT_TRUE(NORTH < SOUTH && SOUTH < EAST && EAST < WEST);
  EXPECT_EQ(WEST - NORTH, 3);
}

TEST(BoardTest, init_from_string) {
  Board b("1001");
  EXPECT_TRUE(b.contains(0) && b.contains(3) && !b.contains(1) && !b.contains(2) && !b.contains(4));
  // Board b("1111");
  EXPECT_EQ(Board("1111"), 15);
  EXPECT_EQ(Board("1"), 1);
  EXPECT_EQ(Board("0000000000000000000000000000000000000000000000000000000000000000"), 0);
  EXPECT_EQ(Board("1111111111111111111111111111111111111111111111111111111111111111"), ULONG_MAX);
  EXPECT_EQ(Board("0101010100000101"), 0xa0aa);
}

TEST(BoardTest, size) {
  EXPECT_EQ(sizeof(Board), 8);
}

TEST(BoardTest, move) {
  Board b;
  b.add(0);
  Board c = b.move(NORTH);
  EXPECT_TRUE(c.contains(8));
  c.add(0);
  c = c.move(NORTH);
  Board d;
  d.add(8, 16);
  EXPECT_EQ(c, d);
}

TEST(BoardTest, add_file_rank) {
  Board b;
  b.add_file_rank('C', 3);
  EXPECT_TRUE(b.contains(18));
  b.add_file_rank('F', 3);
  EXPECT_TRUE(b.contains(21));
  b.add_file_rank('C', 6);
  EXPECT_TRUE(b.contains(42));
  b.add_file_rank('F', 6);
  EXPECT_TRUE(b.contains(45));
}

TEST(BoardTest, move_north_edge) {
  EXPECT_EQ(Board::rank_n(8).move(NORTH), 0);
}

TEST(BoardTest, move_south_edge) {
  EXPECT_EQ(Board::rank_n(1).move(SOUTH), 0);
}

TEST(BoardTest, move_east_edge) {
  EXPECT_EQ(Board::file_n(8).move(EAST), 0);
}

TEST(BoardTest, move_west_edge) {
  EXPECT_EQ(Board::file_n(1).move(WEST), 0);
}

TEST(BoardTest, move_files_east) {
  for(int i=1; i < 8; ++i) {
    EXPECT_EQ(Board::file_n(i).move(EAST), Board::file_n(i+1));
  }
}

TEST(BoardTest, move_files_west) {
  for(int i=8; i > 1; --i) {
    EXPECT_EQ(Board::file_n(i).move(WEST), Board::file_n(i-1));
  }
}

TEST(BoardTest, move_ranks_north) {
  for(int i=1; i < 8; ++i) {
    EXPECT_EQ(Board::rank_n(i).move(NORTH), Board::rank_n(i+1));
  }
}

TEST(BoardTest, move_ranks_south) {
  for(int i=2; i <= 8; ++i) {
    EXPECT_EQ(Board::rank_n(i).move(SOUTH), Board::rank_n(i-1));
  }
}

TEST(BoardTest, is_adjacent) {
  EXPECT_EQ(Board("01").move_opp(EAST), Board("1"));
  EXPECT_EQ(is_adjacent(Board("1"), Board("01"), EAST), Board("1"));
  EXPECT_EQ(is_adjacent(Board("01"), Board("1"), WEST), Board("01"));
  EXPECT_EQ(is_adjacent(Board().add(1), Board().add(9), NORTH), Board().add(1));
  EXPECT_EQ(is_adjacent(Board().add(10), Board().add(2), SOUTH), Board().add(10));
  EXPECT_EQ(Board().add(8).move_opp(NORTH) & Board().add(0), Board().add(0));
}

// Tests Factorial().

// Tests factorial of negative numbers.
// TEST(FactorialTest, Negative) {
// // This test is named "Negative", and belongs to the "FactorialTest"
// // test case.
// EXPECT_EQ(1, Factorial(-5));
// EXPECT_EQ(1, Factorial(-1));
// EXPECT_GT(Factorial(-10), 0);

// // <TechnicalDetails>
// //
// // EXPECT_EQ(expected, actual) is the same as
// //
// //   EXPECT_TRUE((expected) == (actual))
// //
// // except that it will print both the expected value and the actual
// // value when the assertion fails.  This is very helpful for
// // debugging.  Therefore in this case EXPECT_EQ is preferred.
// //
// // On the other hand, EXPECT_TRUE accepts any Boolean expression,
// // and is thus more general.
// //
// // </TechnicalDetails>
// }

// // Tests factorial of 0.
// TEST(FactorialTest, Zero) {
// EXPECT_EQ(1, Factorial(0));
// }

// // Tests factorial of positive numbers.
// TEST(FactorialTest, Positive) {
// EXPECT_EQ(1, Factorial(1));
// EXPECT_EQ(2, Factorial(2));
// EXPECT_EQ(6, Factorial(3));
// EXPECT_EQ(40320, Factorial(8));
// }


// // Tests IsPrime()

// // Tests negative input.
// TEST(IsPrimeTest, Negative) {
// // This test belongs to the IsPrimeTest test case.

// EXPECT_FALSE(IsPrime(-1));
// EXPECT_FALSE(IsPrime(-2));
// EXPECT_FALSE(IsPrime(INT_MIN));
// }

// // Tests some trivial cases.
// TEST(IsPrimeTest, Trivial) {
// EXPECT_FALSE(IsPrime(0));
// EXPECT_FALSE(IsPrime(1));
// EXPECT_TRUE(IsPrime(2));
// EXPECT_TRUE(IsPrime(3));
// }

// // Tests positive input.
// TEST(IsPrimeTest, Positive) {
// EXPECT_FALSE(IsPrime(4));
// EXPECT_TRUE(IsPrime(5));
// EXPECT_FALSE(IsPrime(6));
// EXPECT_TRUE(IsPrime(23));
// }
