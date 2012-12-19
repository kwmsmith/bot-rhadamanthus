CXXFLAGS = -O3 -Wall -pedantic

OBJS = board.o gamestate.o testgamestate.o
TESTS = testgamestate.x

all: $(OBJS) $(TESTS)

testgamestate.x : $(OBJS)
	g++ -o testgamestate.x board.o gamestate.o testgamestate.o

python:
	python setup.py build_ext --inplace

clean:
	rm -r build sipboardBoard.cpp sipboardcmodule.cpp sipAPIboard.h *.so *.o *.pyc
