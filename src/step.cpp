#include "step.h"

using namespace std;

int action_from_char(const char ch)
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
    case 'x':
    case 'X':
      return CAPTURE;
    case 'a':
    case 'A':
      return ADD;
    default:
      return (int)invalid_action();
  }
  assert(0); // should never get here.
}

bool parse_action_str(const string& ss, 
    unsigned char *color,
    unsigned char *piece,
    unsigned char *position,
    unsigned char *action) 
{
  *color = invalid_color();
  *piece = invalid_piece();
  *position = invalid_position();
  *action = invalid_action();

  if (ss.length() < 3) return false;
  if (piece_from_char(ss[0]) == kInvalidPiece) {
    return false;
  } else {
    *piece = piece_from_char(ss[0]);
    *color = color_from_char(ss[0]);
  }
  if (ss[1] < 'a' || ss[1] > 'h') return false;
  if (ss[2] < '1' || ss[2] > '8') return false;

  *position = (ss[2] - '1') * 8 + (ss[1] - 'a');
  if (*position > 63) return false;

  if (ss.length() == 3) {
    *action = ADD;
    return true; // a placement action.
  }

  if (ss.length() != 4) return false;
  *action = action_from_char(ss[3]);
  if (*action == invalid_action()) return false;
  return true;
}

const std::string Step::to_std_string() const 
{
  std::string s;
  if (get_color() == W) {
    switch(get_piece()) {
      case R:
        s += "R";
        break;
      case C:
        s += "C";
        break;
      case D:
        s += "D";
        break;
      case H:
        s += "H";
        break;
      case M:
        s += "M";
        break;
      case E:
        s += "E";
        break;
    }
  } else {
    switch(get_piece()) {
      case R:
        s += "r";
        break;
      case C:
        s += "c";
        break;
      case D:
        s += "d";
        break;
      case H:
        s += "h";
        break;
      case M:
        s += "m";
        break;
      case E:
        s += "e";
        break;
    }
  }
  s += 'a' + get_position() % 8;
  s += '1' + get_position() / 8;
  switch(get_action()) {
    case NORTH:
      s += 'n';
      break;
    case SOUTH:
      s += 's';
      break;
    case EAST:
      s += 'e';
      break;
    case WEST:
      s += 'w';
      break;
    case CAPTURE:
      s += 'x';
      break;
  }
  return s;
}
