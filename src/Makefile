CXX = g++
BOOST_INCL = $(HOME)/Devel/boost_1_52_0
INCLUDE = -I$(BOOST_INCL)
CXXFLAGS = -O3 -Wall -pedantic -Wno-format -Wno-long-long $(INCLUDE)

# PROFILE_LIB = -L$(HOME)/Devel/gperftools-install/lib -lprofiler
# CXXFLAGS = -g -O1 -Wall -pedantic -Wno-format -Wno-long-long $(INCLUDE)

OBJS = board.o step.o gamestate.o arimaa_archive.o transposition.o move.o
TEST_SRC = test_board.cpp test_step.cpp test_gamestate.cpp test_archive.cpp test_transposition.cpp test_move.cpp

GTEST_DIR = /Users/ksmith/Devel/googletest
GTEST_INC = $(GTEST_DIR)/include

python:
	python setup.py build_ext --inplace
	
zobrist_array.include : transposition.cpp
	python zobrist_include.py 768 > $@
	
transposition.o : transposition.cpp transposition.h zobrist_array.include
	$(CXX) $(CXXFLAGS) -c transposition.cpp -o $@

libgtest.a:
	$(CXX) -I$(GTEST_INC) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -I$(GTEST_INC) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc
	$(AR) -rv libgtest.a gtest-all.o gtest_main.o

test_board.x: test_board.cpp board.o libgtest.a
	$(CXX) -I${GTEST_INC} $^ -o $@

test_gamestate.x: test_gamestate.cpp gamestate.o libgtest.a
	$(CXX) -I${GTEST_INC} $^ -o $@
	
test_transposition.x: test_transposition.cpp transposition.o libgtest.a
	$(CXX) -I${GTEST_INC} $^ -o $@

test_all.x : $(OBJS) $(TEST_SRC) libgtest.a board.h step.h
	$(CXX) $(INCLUDE) -I$(GTEST_INC)  $^ -o $@

test: test_all.x
	./test_all.x
	
test-valgrind: test_all.x
	valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes ./test_all.x

clean:
	rm -r build sipboard*.cpp sipAPIboard.h *.so *.o *.pyc *.x

cleanall: clean
	rm libgtest.a
