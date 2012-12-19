CXX = g++
CXXFLAGS = -O3 -Wall -pedantic

OBJS = board.o gamestate.o testgamestate.o
TESTS = testgamestate.x

GTEST_DIR = ../gtest

all: $(OBJS) $(TESTS)

testgamestate.x : $(OBJS)
	$(CXX) -o testgamestate.x board.o gamestate.o testgamestate.o

python:
	python setup.py build_ext --inplace

libgtest.a:
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -I$(GTEST_DIR)/include -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc
	$(AR) -rv libgtest.a gtest-all.o gtest_main.o

try_test.x: try_test.cc board.h board.cpp libgtest.a
	$(CXX) -I${GTEST_DIR}/include try_test.cc libgtest.a -o try_test.x

clean:
	rm -r build sipboard*.cpp sipAPIboard.h *.so *.o *.pyc *.x
