#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "gamestate.h"
#include "move.h"
#include "util.h"

using namespace std;

const vector<string> load_goal_tests(const std::string& fname)
{
  ifstream fs(fname);
  stringbuf ss;
  fs >> &ss;
  return split(ss.str(), "end\n", false);
}

TEST(test_goal, goaltests_LONGRUN)
{
  const vector<string> tests(load_goal_tests("./endgame_positions.txt"));
  for (auto& test : tests) {
    int num_goal = -2;
    auto gs = gamestate_from_goal_position(test, &num_goal);
    auto moves = std::vector<Move>();
    generate_unique_moves(gs, &moves);
    // TODO: check_goal() on the moves; verify the cangoal condition...
  }
}

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
  auto moves = std::vector<Move>();
  generate_unique_moves(gs, &moves);

  int hit_goal = -1;
  for(auto& move : moves) {
    auto& gs = move.get_gamestate();
    if (check_goal(gs, gs.get_color())) {
      hit_goal = 4 - move.get_stepsleft();
      break;
    }
  }
  EXPECT_EQ(hit_goal, num_goal);
}
