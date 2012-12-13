import unittest
import board
import sys

class TestBoard(unittest.TestCase):

    def test_architecture(self):
        'Is this a 64-bit Python?'
        self.assertEqual(sys.maxint, (1<<63) - 1)

    def test_board(self):
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
