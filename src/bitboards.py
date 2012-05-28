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
DIRECTIONS = 'NSEW'
ACTIONS = DIRECTIONS + 'X'

EMPTY_BOARD = 0

def set_board(board, col_rows):
    for c,r in col_rows:
        board |= (_bbpsn_from_col_row(c, int(r)))
    return board

col_to_num = dict(zip(COLUMNS, range(8)))
def _bbpsn_from_col_row(col, row):
    ''' col is in COLUMNS; row is in [1..8].'''
    return 1<<((row-1) * 8 + col_to_num[col])

def _step_dir(board, dir):
    EDGE, OEDGE, shift = _dir_to_edge_and_shift[dir]
    moveable = board & ~EDGE
    if shift > 0:
        return ~OEDGE & ((board & EDGE) | (moveable << shift))
    else:
        return ~OEDGE & ((board & EDGE) | (moveable >> -shift))

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
    moved = _step_dir(psn, direction)
    bboards[color] &= ~psn
    bboards[color] |= moved
    bboards[piece] &= ~psn
    bboards[piece] |= moved

def move_north(board):
    ''' Move every piece on board north.
    Pieces at top of board will stay put.
    '''
    return _step_dir(board, 'N')

def move_south(board):
    return _step_dir(board, 'S')

def move_east(board):
    return _step_dir(board, 'E')

def move_west(board):
    return _step_dir(board, 'W')

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

    bbpsn = _bbpsn_from_col_row(col, row)
    color = 'w' if piece in WPIECES else 'b'
    bbpiece = piece.lower()

    if action == 'X':
        _remove_piece(bbpsn, color, bbpiece, bboards)
    elif action in DIRECTIONS:
        _move_piece(bbpsn, color, bbpiece, bboards, action)
    else:
        raise ValueError('action "{}" not one of "{}".'.format(action, ACTIONS))

def gen_action(color_to_move, stepscompl, bboards):
    '''
    Generates and returns a list of all valid actions that can be taken by
    `color_to_move`.  An action is a sequence of simple steps or captures that
    together comprise a logical change to the board state.  These actions can
    be:
    
      * a push (2 steps), 
      * a pull (2 steps),
      * a simple step (1 step, no push/pull), or
      * a capture (0 steps).
      
    If one or more captures result from a push, pull or simple step, those
    captures will be included in the action list returned.

    Parameters
    ----------
    color_to_move -- either 'w' or 'b'
    stepscompl -- integer, 0..3.  The number of steps already completed.
      If stepscompl is 3, only single steps and resulting captures can be
      generated (no pushes or pulls).
    bboards -- dict of <color or piece> => <bit board> mapping.

    Returns
    -------
    A list of all valid actions that can be taken by side `color_to_move`.
    Each action is specified as a list of named tuples:
    
      (position, action)

    Where position is a bitboard position (1<<0, 1<<1, 1<<2, ..., 1<<63) and
    action is in 'NSEWX'.

    '''
    assert 0 <= stepscompl <= 3

    actions = []

    if stepscompl < 3:
        # generate all pushes.
        actions += _gen_pushes(color_to_move, bboards)
        # generate all pulls.
        actions += _gen_pulls(color_to_move, bboards)
    # generate all simple steps.
    actions += _gen_simple_steps(color_to_move, bboards)
    return actions

def _gen_pushes(color_to_move, bboards):
    raise NotImplementedError("finish me!!!")

def _gen_pulls(color_to_move, bboards):
    raise NotImplementedError("finish me!!!")

def _gen_simple_steps(color_to_move, bboards):
    # A piece p can take a simple step in direction dir iff:
    #  * p is not frozen.
    #  * there exists an adjacent square in dir direction and it is empty.
    simp_steps = []
    not_occupied = ~(bboards['w'] | bboards['b'])
    # find all unfrozen pieces of color_to_move
    unfrozen = _get_unfrozen(color_to_move, bboards)
    empty_map = _empty_adjacent(not_occupied, unfrozen)
    for dir, psns in empty_map.values():
        for psn in board_to_posns(psns):
            simp_steps.append([(psn, dir)])
    for steps in simp_steps:
        raise NotImplementedError("captures!!!")
    return simp_steps

def _gen_capture(bboards, (psn, dir)):
    raise NotImplementedError("finish me!!!")

def _adjacent_with_dir(positions, pieces):
    '''
    Returns a mapping of direction => pieces.  The pieces are adjacent to the
    positions by moving in the direction indicated.

    '''
    ret = {}
    for dir in DIRECTIONS:
        odir = _opp_from_dir[dir]
        steps = _step_dir(positions, odir)
        adj_pieces = pieces & steps
        ret[dir] = adj_pieces
    return ret

def _adjacent(positions, pieces):
    '''
    Returns the subset of pieces that are adjacent to the given positions.

    '''
    ret = EMPTY_BOARD
    for adj in _adjacent_with_dir(positions, pieces).values():
        ret |= adj
    return ret

def _empty_adjacent(empties, board):
    return _adjacent_with_dir(empties, board)

def _free_pieces(color_to_move, bboards):
    unfrozen = EMPTY_BOARD
    color = color_to_move
    ocolor = 'b' if color_to_move == 'w' else 'w'
    empties = ~(bboards['w'] | bboards['b'])
    all_stronger = EMPTY_BOARD
    frozen = EMPTY_BOARD
    e = bboards[color] & bboards['e']
    # FIXME: shouldn't have to special case elephants...
    if _adjacent(empties, e):
        unfrozen |= e
    for piece, stronger in zip('mhdcr', 'emhdc'):
        # all_stronger accumulates the stronger pieces; the
        # positions of all stronger pieces of opposite color.
        opp_stronger = bboards[ocolor] & bboards[stronger]
        all_stronger |= opp_stronger
        these_pieces = bboards[color] & bboards[piece]
        frozen |= _adjacent(all_stronger, these_pieces)
    unfrozen |= bboards[color] & ~frozen
    return unfrozen

def _allowed_steps(color_to_move, bboards):
    '''
    returns a mapping of direction =>
    positions-of-color-allowed-to-move-in-direction.

    '''
    empties = _get_empties(bboards)
    free = _free_pieces(color_to_move, bboards)
    dir_to_pieces = _adjacent_with_dir(empties, free)
    # correct for rabbits.
    excluded_dir = 'N' if color_to_move == 'b' else 'S'
    rabbits = bboards[color_to_move] & bboards['r']
    dir_to_pieces[excluded_dir] &= ~rabbits
    return dir_to_pieces

def _get_empties(bboards):
    return ~(bboards['w'] | bboards['b'])

def _num_pieces(bboard):
    return len(board_to_idxs(bboard))

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
    return [(1<<i) for i in range(64) if (board & (1<<i))]

def board_to_idxs(board):
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

def empty_bboards():
    return dict((ch, 0) for ch in 'wbemhdcr')

def bboards_from_char_state(ch_board):
    ''' Given a standard notation Arimaa board, returns a dictionary of
    <piece label> => <position bboard> entries.

    Parameters
    ----------
    ch_board -- list of characters, or str.  List will be joined.  contains a
        valid Arimaa position.  Squares are given in the order a8-h8, a7-h7, ...,
        a1-h1.

    Returns
    -------
    Dictionary mapping from either piece label ('rcdhme') or color ('wb') to
    bitboard representing position of resp. type.

    '''
    if isinstance(ch_board, (list, tuple)):
        ch_board = ''.join(ch_board)
    piece_posns = empty_bboards()
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
            for idx in board_to_idxs(board):
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

_opp_from_dir = {
        'N': 'S',
        'S': 'N',
        'E': 'W',
        'W': 'E',
}

_dir_to_edge_and_shift = {
        'N': (RANK8, RANK1, 8),
        'S': (RANK1, RANK8, -8),
        'E': (FILEH, FILEA, 1),
        'W': (FILEA, FILEH, -1),
        }
