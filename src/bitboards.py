'''
Functions and classes for manipulating bitboards.

'''

#-----------------------------------------------------------------------------
# Implementation details:
#
# The most sig. bit of a bitboard corresponds to square h8 of an Arimaa board.
# The least sig. bit of a bitboard corresponds to squard a1 of an Arimaa board.
#
# North corresponds to adding 8 to an index (left shifting a bb by 8).
# South corresponds to subtracting 8 from an index(right shifting a bb by 8).
# East corresponds to adding 1 to an index (right shifting a bb by 1).
# West corresponds to subtracting 1 from an index (left shifting a bb by 1).
#
#          a  b  c  d  e  f  g  h
#       +-------------------------+
#     8 | 56 57 58 59 60 61 62 63 | 8
#     7 | 48                   55 | 7
#     6 | 40                   47 | 6
#     5 | 32                   39 | 5
#     4 | 24                   31 | 4
#     3 | 16                   23 | 3
#     2 | 08                   15 | 2
#     1 | 00 01 02 03 04 05 06 07 | 1
#       +-------------------------+
#          a  b  c  d  e  f  g  h
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# --- Legal pieces
#-----------------------------------------------------------------------------
WPIECES = "RCDHME"
BPIECES = WPIECES.lower()
PIECES = BPIECES + WPIECES

COLUMNS = 'abcdefgh'
ROWS = range(1, 8+1)

# NSEW - move a piece in that direction (with White at South, Black at North)
# 'X' - capture a piece.
ACTIONS = 'NSEWX'

EMPTY_BOARD = 0

def set_board(board, col_rows):
    for c,r in col_rows:
        board |= (_bbpsn_from_col_row(c, int(r)))
    return board

col_to_num = dict(zip(COLUMNS, range(8)))
def _bbpsn_from_col_row(col, row):
    ''' col is in COLUMNS; row is in [1..8].'''
    return 1<<((row-1) * 8 + col_to_num[col])

def _move_dir(board, dir):
    EDGE, shift = _dir_to_edge_and_shift[dir]
    moveable = board & ~EDGE
    if shift > 0:
        return (board & EDGE) | (moveable << shift)
    else:
        return (board & EDGE) | (moveable >> -shift)

def _remove_piece(psn, color, piece, bboards):
    ''' Remove piece at psn from the board.  Checks that psn is on one of the
    trap squares.

    psn -- bb position.
    color -- 'w' or 'b'.
    piece -- one of 'rcdhme'.
    bboards -- dict of <color or piece> => <bitboard> mappings.

    '''
    if not psn & TRAPS:
        msg = 'Action is "X" but position is "{}" and is not on a trap'.format
        raise ValueError(msg(psn))

    num_color = _num_pieces(bboards[color])
    num_piece = _num_pieces(bboards[piece])
    bboards[color] &= ~psn
    bboards[piece] &= ~psn
    assert num_color - 1 == _num_pieces(bboards[color])
    assert num_piece - 1== _num_pieces(bboards[piece])

def _move_piece(psn, color, piece, bboards, direction):
    ''' Move piece at psn in direction `direction`.

    psn -- bb position.
    color -- 'w' or 'b'.
    piece -- one of 'rcdhme'.
    bboards -- dict of <color or piece> => <bitboard> mappings.
    direction -- one of 'NSEW'.

    '''
    num_color = _num_pieces(bboards[color])
    num_piece = _num_pieces(bboards[piece])

    _move_piece_fast(psn, color, piece, bboards, direction)

    assert num_color == _num_pieces(bboards[color])
    assert num_piece == _num_pieces(bboards[piece])

def _move_piece_fast(psn, color, piece, bboards, direction):
    moved = _move_dir(psn, direction)
    bboards[color] &= ~psn
    bboards[color] |= moved
    bboards[piece] &= ~psn
    bboards[piece] |= moved

def move_north(board):
    ''' Move every piece on board north.
    Pieces at top of board will stay put.
    '''
    return _move_dir(board, 'N')

def move_south(board):
    return _move_dir(board, 'S')

def move_east(board):
    return _move_dir(board, 'E')

def move_west(board):
    return _move_dir(board, 'W')

def apply_step(stepstr, bboards):
    piece, col, row, action = stepstr
    row = int(row)
    action = action.upper()
    if piece not in PIECES:
        raise ValueError('piece "{}" not one of "{}".'.format(piece, PIECES))
    if col not in COLUMNS:
        raise ValueError('column "{}" not one of "{}".'.format(col, COLUMNS))
    if row not in ROWS:
        raise ValueError('row "{}" not one of "{}".'.format(row, ROWS))
    if action not in ACTIONS:
        raise ValueError('action "{}" not one of "{}".'.format(action, ACTIONS))

    bbpsn = _bbpsn_from_col_row(col, row)
    color = 'w' if piece in WPIECES else 'b'
    bbpiece = piece.lower()

    if action == 'X':
        _remove_piece(bbpsn, color, bbpiece, bboards)
    elif action in 'NSEW':
        _move_piece(bbpsn, color, bbpiece, bboards, action)
    else:
        raise ValueError("Invalid action, given '{}'".format(action))


def _num_pieces(bboard):
    return len(board_to_posns(bboard))

def _reverse_str(s):
    return ''.join(reversed(s))

def bb_repr(board, N=8):
    ''' Returns a nice string representation of a bitboard.
    '''
    res = []
    ctr = N
    fmt = lambda s: _reverse_str(('{'+':0>{N}b'.format(N=N)+'}').format(s))
    least_sig_bits = (1 << N) - 1
    while ctr:
        res.append(fmt(board & least_sig_bits))
        board = (board >> N)
        ctr -= 1
    return '\n'.join(reversed(res))

def board_to_posns(board):
    ''' Given a bitboard, returns a list of 1D board indexes.
    '''
    return [i for i in range(64) if (board & (1<<i))]

def _char_state_iter(ch_board):
    '''
    Iterates through the ch_board in ascending bb index.

    ch_board is given in the order a8-h8, a7-h7, ..., a1-h1.
    '''

    rows = reversed([ch_board[i*8:(i+1)*8] for i in range(8)])
    res = []
    for row in rows:
        res.extend(row)
    return res

def bboards_from_char_state(ch_board):
    ''' Given a standard notation Arimaa board, returns a dictionary of
    <piece label> => <position bboard> entries.

    Parameters
    ----------
    ch_board -- str containing a valid Arimaa position.
        Squares are given in the order a8-h8, a7-h7, ..., a1-h1.

    Returns
    -------
    Dictionary mapping from either piece label ('rcdhme') or color ('wb') to
    bitboard representing position of resp. type.

    '''
    piece_posns = {}
    for i, ch in enumerate(_char_state_iter(ch_board)):
        if ch not in ('rcdhmeRCDHME'):
            continue
        board_idx = (1 << i)
        lch = ch.lower()
        piece_posns[lch] = piece_posns.get(lch, 0) | board_idx
        if ch.islower():
            color = 'b'
        else:
            color = 'w'
        piece_posns[color] = piece_posns.get(color, 0) | board_idx
    return piece_posns

def is_goal(bboards):
    brabbits = bboards['b'] & bboards['r']
    wrabbits = bboards['w'] & bboards['r']
    if brabbits & RANK1:
        return 'b'
    elif wrabbits & RANK8:
        return 'w'
    else:
        return ''

def char_state_from_bboards(bboards, empty=' '):
    ''' Given a dictionary of <piece label> => <position bboard> entries,
    returns a standard notation Arimaa board.
    '''
    ch_board = [empty] * 64
    wps = bboards.get('w', EMPTY_BOARD) # white / gold pieces
    bps = bboards.get('b', EMPTY_BOARD) # black / silver pieces
    for piece in 'rcdhme':
        # get the white / black piece for the piece type.
        bbb = bboards.get(piece, 0) & bps
        wbb = bboards.get(piece, 0) & wps
        # iterate over (piece label, piece locations) tuples.
        for piece, board in ((piece.lower(), bbb),
                             (piece.upper(), wbb)):
            for idx in board_to_posns(board):
                # FIXME: refactor these lines
                # Necessary to correct the column index.
                row, col = divmod(idx, 8)
                col = -col + 7
                idx = 8 * row + col
                assert ch_board[idx] == empty
                ch_board[idx] = piece
    return ''.join(reversed(ch_board))

#-----------------------------------------------------------------------------
# Position constants
#-----------------------------------------------------------------------------

RANK8 = 0
for i in range(56, 64):
    RANK8 |= (1<<i)

RANK1 = 0
for i in range(0, 8):
    RANK1 |= (1<<i)

FILEH = 0
for i in range(7, 64, 8):
    FILEH |= (1<<i)

FILEA = 0
for i in range(0, 64, 8):
    FILEA |= (1<<i)

TRAPS = set_board(0, ['c3', 'f3', 'c6', 'f6'])

_dir_to_edge_and_shift = {
        'N': (RANK8, 8),
        'S': (RANK1, -8),
        'E': (FILEH, 1),
        'W': (FILEA, -1),
        }
