from distutils.core import Extension, setup
import os

os.system("sip -c . board.sip")

ext = Extension("board", 
                sources=["board.cpp", "sipboardBoard.cpp", "sipboardcmodule.cpp"])

setup(name="board", ext_modules=[ext])
