#include "board.h"

// Board::Board(uint64_t board = 0) : _board(board) {}
unsigned int num_directions() {
  static unsigned int num_directions_ = WEST + 1;
  return num_directions_;
}

unsigned int opp_dir(unsigned int dir)
{
  switch(dir) {
    case NORTH:
      return SOUTH;
    case SOUTH:
      return NORTH;
    case EAST:
      return WEST;
    case WEST:
      return EAST;
    default:
      assert(0);
  }
  assert(0);
}

int direction_from_char(const char ch)
{
  switch(ch) {
    case 'n':
    case 'N':
      return NORTH;
    case 's':
    case 'S':
      return SOUTH;
    case 'e':
    case 'E':
      return EAST;
    case 'w':
    case 'W':
      return WEST;
    default:
      assert(0);
  }
  assert(0); // should never get here.
}
