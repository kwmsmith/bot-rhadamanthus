#include "search.h"
#include "gtest/gtest.h"
#include <string>

using namespace std;

TEST(Search, alphabeta)
{
  GameState gs;
  std::string ss(
      "10w \n"
      " +-----------------+\n"
      "8| r r r r r r r r |\n"
      "7| c c d d h h m e |\n"
      "6|                 |\n"
      "5|                 |\n"
      "4|                 |\n"
      "3|                 |\n"
      "2| C C D D H H M E |\n"
      "1| R R R R R R R R |\n"
      " +-----------------+\n"
      "   a b c d e f g h\n");
  EXPECT_TRUE(gamestate_from_input(ss, &gs));
  EXPECT_EQ(alphabeta(gs, -1000, 1000, 8), 0);
}
