'''
Functions and classes for manipulating bitboards.

'''

#-----------------------------------------------------------------------------
# Implementation details:
#
# The most sig. bit of a bitboard corresponds to square a8 of an Arimaa board.
# 
# North corresponds to adding 8 to an index (left shifting a bb by 8).
# South corresponds to subtracting 8 from an index(right shifting a bb by 8).
# East corresponds to subtracting 1 from an index (right shifting a bb by 1).
# West corresponds to adding 1 to an index (left shifting a bb by 1).
#
#         a  b  c  d  e  f  g  h
#      +-------------------------+
#     8| 63 62 61 60 59 58 57 56 |8
#     7| 55                   48 |7
#     6| 47                   40 |6
#     5| 39                   32 |5
#     4| 31                   24 |4
#     3| 23                   16 |3
#     2| 15                    8 |2
#     1|  7  6  5  4  3  2  1  0 |1
#      +-------------------------+
#         a  b  c  d  e  f  g  h
#
#-----------------------------------------------------------------------------

def move_north(posn, board):
    ''' Move piece at posn one step north.
    '''
    raise Exception("test me!")
    assert (board & posn)
    return (board ^ posn) | (board | (posn << 8))

def bb_repr(board, N=8):
    ''' Returns a nice string representation of a bitboard.
    '''
    res = []
    ctr = N
    fmt = ('{'+':0>{N}b'.format(N=N)+'}').format
    least_sig_bits = (1 << N) - 1
    while ctr:
        res.append(fmt(board & least_sig_bits))
        board = board >> N
        ctr -= 1
    return '\n'.join(reversed(res))

def board_to_posns(board):
    ''' Given a bitboard, returns a list of 1D board indexes.
    '''
    return [i-1 for i in range(64) if (board & (1<<i))]

def bboards_from_char_state(ch_board):
    ''' Given a standard notation Arimaa board, returns a dictionary of 
    <piece label> => <position bboard> entries.
    '''
    piece_posns = {}
    for i, ch in enumerate(reversed(ch_board)):
        if ch not in ('rcdhmeRCDHME'):
            continue
        board_idx = 1<<i
        piece_posns[ch] = piece_posns.get(ch, 0) | board_idx
    return piece_posns

def char_state_from_bboards(bboards):
    ''' Given a dictionary of <piece label> => <position bboard> entries,
    returns a standard notation Arimaa board.
    '''
    ch_board = ['-'] * 64
    for piece, board in bboards.items():
        for idx in board_to_posns(board):
            assert ch_board[idx] == '-'
            ch_board[idx] = piece
    return ''.join(reversed(ch_board))
