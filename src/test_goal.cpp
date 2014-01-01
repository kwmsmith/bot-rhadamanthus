#include "gamestate.h"
#include "gtest/gtest.h"
#include <string>

using namespace std;

TEST(test_goal, initialize)
{
  std::string ss(
      "g\n"
      " +-----------------+\n"
      "8| . . . r . r r r |\n"
      "7| . . r C . d . . |\n"
      "6| r h x e R x m . |\n"
      "5| . . . . R . . . |\n"
      "4| . . . . . . . . |\n"
      "3| . . x . . x . . |\n"
      "2| . . . . . . . . |\n"
      "1| R . . E H . . . |\n"
      " +-----------------+\n"
      "   a b c d e f g h \n"
      "\n"
      "::: cangoal 2\n");
  int num_goal = -1;
  auto gs = gamestate_from_goal_position(ss, &num_goal);
  EXPECT_EQ(num_goal, 2);
  gs.clear();
}
