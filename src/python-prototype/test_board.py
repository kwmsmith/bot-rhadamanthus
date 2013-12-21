import unittest
import board
import sys

class TestBoard(unittest.TestCase):

    def test_architecture(self):
        'Is this a 64-bit Python?'
        self.assertEqual(sys.maxint, (1<<63) - 1)

    def test_board_value_range(self):
        '''Can all values 2**i for i in range(64) be held in the board?'''
        b = board.Board()
        for i in range(64):
            b._board = 1<<i
            self.assertEqual(b._board, 1<<i, i)

    def test_hold_max(self):
        'Can a board hold the maxint value, and nothing greater?'
        b = board.Board()
        b._board = (1<<63) - 1
        self.assertEqual(b._board, sys.maxint)
        b._board = 1<<64
        self.assertEqual(b._board, 0)

    def test_set_value(self):
        'Can we set each value and test for containment?'
        for i in range(64):
            b = board.Board()
            b.add(i)
            self.assertTrue(b.contains(i), (b._board, i))

    def test_add_everything(self):
        b = board.Board()
        for i in range(64):
            b.add(i)
        self.assertEqual(b._board, (1<<64)-1, (bin(b._board),))

    def test_remove_value(self):
        b = board.Board()
        b.add(5)
        b.remove(5)
        self.assertEqual(b._board, 0)

    def test_add_remove_all(self):
        b = board.Board()
        for i in range(64):
            b.add(i)
        for i in range(64):
            b.remove(i)
        self.assertEqual(b._board, 0)

    def _test_update(self):
        b = board.Board()
        b.update(range(64))
        self.assertEqual(b._board, (1<<64)-1)

    def _test_free_to_move(self):
        pass
