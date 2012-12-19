#include "gamestate.h"
#include <iostream>

int main(int argc, char **argv)
{
    GameState gb = GameState();
    GameState gb2 = GameState();
    Board b = gb2.mobile_pieces(W);
    std::cout << "foobar " << b._board << std::endl;
    gb2 = gb;
    return 0;
}
