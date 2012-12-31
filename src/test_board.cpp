#include "board.h"
#include "gtest/gtest.h"
#include <climits>
#include <cstdio>

TEST(BoardTest, Instantiate) {
    Board b;
    EXPECT_EQ(b, 0ULL);
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

TEST(BoardTest, move_north_edge) {
    Board b;
    for(int i=64-8; i < 64; i++)
        b.add(i);
    b = b.move(NORTH);
    EXPECT_EQ(b, 0);
}

TEST(BoardTest, move_south_edge) {
    Board b;
    for(int i=0; i < 8; i++)
        b.add(i);
    b = b.move(SOUTH);
    EXPECT_EQ(b, 0);
}

TEST(BoardTest, move_east_edge) {
    Board b;
    for(int i=7; i < 64; i+=8)
        b.add(i);
    b = b.move(EAST);
    EXPECT_EQ(b, 0);
}

TEST(BoardTest, move_west_edge) {
    Board b;
    for(int i=0; i < 64; i+=8)
        b.add(i);
    b = b.move(WEST);
    EXPECT_EQ(b, 0);
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
