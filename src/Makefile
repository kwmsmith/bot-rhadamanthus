BOOST_INCL = $(HOME)/Devel/boost_1_52_0
INCLUDE = -I$(BOOST_INCL)
CXX = g++
CXXFLAGS = -O3 -Wall -pedantic -Wno-format $(INCLUDE)

OBJS = board.o gamestate.o arimaa_archive.o
TESTS = test_gamestate.x test_board.x
TEST_SRC = test_board.cpp test_gamestate.cpp test_archive.cpp

GTEST_DIR = ../gtest
GTEST_INC = $(GTEST_DIR)/include

all: $(OBJS) $(TESTS)

python:
	python setup.py build_ext --inplace

libgtest.a:
	$(CXX) -I$(GTEST_INC) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -I$(GTEST_INC) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc
	$(AR) -rv libgtest.a gtest-all.o gtest_main.o

test_board.x: test_board.cpp board.o libgtest.a
	$(CXX) -I${GTEST_INC} $^ -o $@

test_gamestate.x: test_gamestate.cpp gamestate.o libgtest.a
	$(CXX) -I${GTEST_INC} $^ -o $@

test_all.x : $(OBJS) $(TEST_SRC) libgtest.a board.h
	$(CXX) $(INCLUDE) -I$(GTEST_INC) $^ -o $@

test: test_all.x
	./test_all.x

clean:
	rm -r build sipboard*.cpp sipAPIboard.h *.so *.o *.pyc *.x

cleanall: clean
	rm libgtest.a
