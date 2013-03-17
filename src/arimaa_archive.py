import bitboards
import tarfile

#-----------------------------------------------------------------------------
# The ordered fields inside an Arimaa archive file.
#-----------------------------------------------------------------------------
FIELDS = [
            'id',
            'wplayerid',
            'bplayerid',
            'wusername',
            'busername',
            'wtitle',
            'btitle',
            'wcountry',
            'bcountry',
            'wrating',
            'brating',
            'wratingk',
            'bratingk',
            'wtype',
            'btype',
            'event',
            'site',
            'timecontrol',
            'postal',
            'startts',
            'endts',
            'result',
            'termination',
            'plycount',
            'mode',
            'rated',
            'corrupt',
            'movelist',
            'events',
]

EMPTY_BOARD = ' ' * 64

#-----------------------------------------------------------------------------
# Utility functions to convert a string field to correct representation.
#-----------------------------------------------------------------------------

def _noop(arg):
    return unicode(arg)

def _convert_to_int(ss):
    return int(ss)

def _convert_to_pos_int(ss):
    ii = int(ss)
    if ii < 0:
        raise ValueError("value of {} < 0.".format(id))
    return ii

def _convert_to_boolean(ss):
    b = bool(int(ss))
    return b

def convert_movelist(ml):
    ml = unicode(ml)
    moves = ml.split(u'\\n')
    moves = [m.split() for m in moves]
    return moves

def convert_events(evts):
    evts = unicode(evts)
    return evts.split('\\n')

def convert_result(r):
    if r not in 'bwud':
        msg = "Result must be 'b' (black/silver), 'w' (white/gold), 'u', or 'd', but got '{}'."
        raise ValueError(msg.format(r))
    return r

def convert_type(tp):
    if tp not in 'bh':
        msg = "Player type must be 'b' (bot) or 'h' (human), got '{}' instead."
        raise ValueError(msg.format(tp))
    return tp

#
# Maps field name to appropriate converter.
#
FIELD_CONVERTERS = {
            'id': _convert_to_pos_int,
            'wplayerid': _convert_to_pos_int,
            'bplayerid': _convert_to_pos_int,
            'wusername': _noop,
            'busername': _noop,
            'wtitle': _noop,
            'btitle': _noop,
            'wcountry': _noop,
            'bcountry': _noop,
            'wrating': _convert_to_pos_int,
            'brating': _convert_to_pos_int,
            'wratingk': _convert_to_int,
            'bratingk': _convert_to_int,
            'wtype': convert_type,
            'btype': convert_type,
            'event': _noop,
            'site': _noop,
            'timecontrol': _noop,
            'postal': _convert_to_boolean,
            'startts': _noop,
            'endts': _noop,
            'result': convert_result,
            'termination': _noop,
            'plycount': _convert_to_pos_int,
            'mode': _noop,
            'rated': _convert_to_boolean,
            'corrupt': _convert_to_boolean,
            'movelist': convert_movelist,
            'events': convert_events,
        }

#-----------------------------------------------------------------------------
# Public API.
#-----------------------------------------------------------------------------

def get_fh(fname):
    try:
        fh = tarfile.open(fname, 'r:gz').fileobj
    except tarfile.ReadError:
        fh = open(fname, 'r')
    return fh

def fields_from_archive(fname):
    ''' Given an Arimaa gamefile, returns the fields for the game DB.
    '''
    fh = get_fh(fname)
    raw_header = fh.readline()
    header = raw_header.split() 
    if header != FIELDS:
        msg = "Fields from file '{}' not equal to expected fields.".format
        raise ValueError(msg(fname))
    return header

def games_from_archive(fname):
    ''' Given a tgz'd Arimaa gamefile, returns an iterator over the games in a file.
    '''
    fields = fields_from_archive(fname)
    fh = get_fh(fname)
    # skip the header line
    fh.readline()
    for game_string in fh:
        if set(game_string) == {'\x00'}:
            break
        yield game_from_game_string(fields, game_string)

def game_from_game_string(fields, game_string):
    game_list = game_string.split('\t')
    if len(game_list) != len(fields):
        msg = "Game entry with ID {} does not have the correct number of fields"
        raise ValueError(msg.format(game_list[0]))
    game = dict(zip(fields, game_list))
    # Run the values through the converters
    for field, ss in game.items():
        game[field] = FIELD_CONVERTERS[field](ss)
    return game

def play_archive_game(allmoves):
    '''
    allmoves is a list starting with 1w, 1b (the game setup moves), followed by
    2w, 2b and following (the move playouts).

    '''

    if len(allmoves) < 2:
        return bitboards.bboards_from_char_state(EMPTY_BOARD)
    wmove1, bmove1 = allmoves[:2]
    board = game_setup_from_move_1(wmove1, bmove1)
    bbs = bitboards.bboards_from_char_state(board)
    for move in allmoves[2:]:
        label, steps = move[0], move[1:]
        for step in steps:
            bitboards.apply_step(step, bbs)
    return bbs

def game_setup_from_move_1(wmove1, bmove1):

    setup = ['-'] * 64

    def _place_wb(move1):
        if move1[0] not in ('1w', '1b'):
            raise ValueError()
        color = move1[0][-1]
        pieces = 'RCDHME'
        if color == 'b':
            pieces = pieces.lower()
        places = move1[1:]
        if len(places) != 16:
            raise ValueError()
        for pl in places:
            piece, col, row = pl
            col = col.lower()
            row = int(row)
            if piece not in pieces:
                raise ValueError()
            if col not in 'abcdefgh':
                raise ValueError()
            if not 1 <= row <= 8:
                raise ValueError()
            col = ord(col) - ord('a')
            row -= 1
            # a8 corresponds to index 0; adjust accordingly.
            # slope of neg one, intercept of 7.
            row = flip_row_idx(row)
            idx = col + 8 * row
            setup[idx] = piece

    _place_wb(wmove1)
    _place_wb(bmove1)

    tp_to_count = {
            'r': 8,
            'c': 2,
            'd': 2,
            'h': 2,
            'm': 1,
            'e': 1,
            '-': 32,
            }

    for tp, cnt in tp_to_count.items():
        if setup.count(tp) != cnt or setup.count(tp.upper()) != cnt:
            raise ValueError()

    return ''.join(setup)

def flip_row_idx(idx):
    return -idx + 7

def pprint_from_board_str(board_str):
    board_str = board_str.replace('-', ' ')
    board_lst = list(board_str)

    board_rows = []
    for i in range(8):
        row_idx = flip_row_idx(i)
        row_idx += 1
        row_idx = str(row_idx)
        board_rows.append([row_idx, '|'] +
                          board_lst[8*i:8*(i+1)] + 
                          ['|', row_idx])

    col_labels = list('  abcdefgh  ')
    row_border = list(' +--------+ ')
    board_rows.insert(0, row_border)
    board_rows.insert(0, col_labels)
    board_rows.append(row_border)
    board_rows.append(col_labels)

    board_rows = [' '.join(row) for row in board_rows]
    ppboard = '\n'.join(board_rows)
    return ppboard
