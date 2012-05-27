import unittest
import bitboards as bb


class TestBitboards(unittest.TestCase):

    def setUp(self):
        board = 0
        for i in range(0, 64, 9):
            board |= (1<<i)
        self.diag_board = board

    def _test_move_bug(self):
        # TODO: if move south, the north rank must be empty.
        # Test for ~RANK8, ~RANK1; the bit-flip does weird stuff with two's
        # compliment / neg numbers.
        pass

    def test_move_north(self):
        self.assertEquals(bb.move_north(bb.RANK8), bb.RANK8)

        nboard = 0
        for i in range(8, 64, 9):
            nboard |= (1<<i)
        nboard |= (1<<63)
        self.assertEquals(bb.move_north(self.diag_board), nboard)

    def test_move_south(self):
        self.assertEquals(bb.move_south(bb.RANK1), bb.RANK1)

        sboard = 0
        for i in range(1, 64, 9):
            sboard |= (1<<i)
        sboard |= 1
        self.assertEquals(bb.move_south(self.diag_board), sboard)

    def test_move_east(self):
        self.assertEquals(bb.move_east(bb.FILEH), bb.FILEH)

        eboard = 0
        for i in range(1, 64, 9):
            eboard |= (1<<i)
        eboard |= (1<<63)
        self.assertEquals(bb.move_east(self.diag_board), eboard)

    def test_move_west(self):
        self.assertEquals(bb.move_west(bb.FILEA), bb.FILEA)

        wboard = 0
        for i in range(8, 64, 9):
            wboard |= (1<<i)
        wboard |= (1<<0)
        self.assertEquals(bb.move_west(self.diag_board), wboard)

    def test_bb_repr(self):
        self.assertEquals(bb.bb_repr((1<<64) - 1), '''\
11111111
11111111
11111111
11111111
11111111
11111111
11111111
11111111\
''')
        self.assertEquals(bb.bb_repr(0), '''\
00000000
00000000
00000000
00000000
00000000
00000000
00000000
00000000\
''')
        self.assertEquals(bb.bb_repr(self.diag_board), '''\
00000001
00000010
00000100
00001000
00010000
00100000
01000000
10000000\
''')


    def test_board_to_positions(self):
        bd = (1)
        self.assertEquals(bb.board_to_idxs(bd), [0])
        for i in range(64):
            self.assertEquals(bb.board_to_idxs((1<<i)), [i])
            if not i:
                continue
            for j in range(i):
                self.assertEquals(bb.board_to_idxs((1<<i) | (1<<j)), [j, i])
                

    def test_bb_to_from_ch_state(self):
        # corner case: empty board.
        ch_state = ' ' * 64
        psns = bb.bboards_from_char_state(ch_state)
        self.assertEquals(psns, dict((ch, 0) for ch in 'wbemhdcr'))
        self.assertEquals(ch_state, bb.char_state_from_bboards(psns))

        # test all piece types.
        ch_state = [' '] * 64
        pieces = 'rcdhmeRCDHME'
        for idx, p in enumerate(pieces, 10):
            ch_state[idx] = p
        psns = bb.bboards_from_char_state(ch_state)
        self.assertEquals(''.join(ch_state), bb.char_state_from_bboards(psns))

class TestGen(unittest.TestCase):

    def setUp(self):
        ch_state = [' '] * 64
        ch_state[0] = 'e'
        self.ch_state = ch_state
        self.bbds = bb.bboards_from_char_state(ch_state)

    def test_adjacent(self):
        empties = bb._get_empties(self.bbds)
        adj = bb._adjacent_with_dir(empties, self.bbds['b'])
        self.assertTrue(adj['S'])
        self.assertTrue(adj['E'])
        self.assertFalse(adj['N'])
        self.assertFalse(adj['W'])

    def test_gen_simple_steps(self):
        loc = (1<<56)
        self.assertTrue(loc & bb._free_pieces('b', self.bbds))
        dir_to_psns = dict(S=loc, E=loc, N=0, W=0)
        self.assertEquals(dir_to_psns, bb._allowed_steps('b', self.bbds))
