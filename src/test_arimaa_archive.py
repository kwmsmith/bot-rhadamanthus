from arimaa_archive import (games_from_compressed, game_setup_from_move_1, 
                            pprint_from_board_str)

def test_games_from_compressed():
    for game in games_from_compressed('allgames201111.tgz'):
        moves = game['movelist']
        # assert len(moves) == 2 * game['plycount']

        if len(moves) < 2:
            continue
        move1w, move1b = moves[:2]
        board = game_setup_from_move_1(move1w, move1b)
        print pprint_from_board_str(board)
