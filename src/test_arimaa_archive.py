from arimaa_archive import (games_from_compressed, game_setup_from_move_1, 
                            pprint_from_board_str, EMPTY_BOARD)
import bitboards as bb
from bitboards import bboards_from_char_state

from os import path

ALLGAMESFILE = path.join(path.dirname(__file__), 'allgames201111.tgz')

def _test_game_setup(game):
    moves = game['movelist']
    if len(moves) < 2:
        return EMPTY_BOARD
    move1w, move1b = moves[:2]
    board = game_setup_from_move_1(move1w, move1b)
    return board

def _test_play_game(game):
    board = _test_game_setup(game)
    bboards = bboards_from_char_state(board)
    moves = game['movelist'][2:]
    if len(moves) < 2:
        return
    for move in moves:
        for step in move[1:]:
            piece, col, row, direction = step
            lpiece = piece.lower()
            if direction in 'xX':
                continue
            bboards[lpiece] = bb._move_dir(bboards[lpiece], direction)

def test_games_from_compressed():
    for game in games_from_compressed(ALLGAMESFILE):
        board = _test_game_setup(game)
        "Game ID:", game['id']
        pprint_from_board_str(board)
        _test_play_game(game)
