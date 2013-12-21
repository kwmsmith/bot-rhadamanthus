from arimaa_archive import (games_from_compressed, game_setup_from_move_1, 
                            pprint_from_board_str, EMPTY_BOARD, play_archive_game)
from bitboards import char_state_from_bboards, is_goal

from os import path

ALLGAMESFILE = path.join(path.dirname(__file__), 'allgames201111.tgz')

def _test_game_setup(game):
    moves = game['movelist']
    if len(moves) < 2:
        return EMPTY_BOARD
    move1w, move1b = moves[:2]
    board = game_setup_from_move_1(move1w, move1b)
    return board

def test_games_from_compressed():

    for game in games_from_compressed(ALLGAMESFILE):
        board = _test_game_setup(game)
        header = '#' + '-' * 79
        print header
        print "Game ID:", game['id']
        print pprint_from_board_str(board)
        allmoves = game['movelist']
        end_bb = play_archive_game(allmoves)
        board = char_state_from_bboards(end_bb)
        print
        print pprint_from_board_str(board)

        print "TERMINATION: {}".format(game['termination'])

        if game['termination'].lower() == 'g':
            tg = is_goal(end_bb)
            assert tg == game['result']
            print "GOAL: {}".format(game['result'])

        print header
