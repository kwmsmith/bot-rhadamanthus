#-----------------------------------------------------------------------------
# Compiler and flags.
#-----------------------------------------------------------------------------
# CXX = g++

CXX = clang++ 
CXX11_FLAGS = -std=c++11 -stdlib=libc++ 
CLANG_FLAGS = -Werror -nostdinc++ \
			  -I/Users/ksmith/Devel/llvm-libcxx-svn/include

CXXFLAGS = $(CXX11_FLAGS) $(CLANG_FLAGS) -Wno-narrowing -O3 -Wall \
		   -pedantic -Wno-format -Wno-long-long 
LDFLAGS =  $(CXX11_FLAGS) -L/Users/ksmith/Devel/llvm-libcxx-svn/lib

SPARSEHASH_INCL = sparsehash/include
SPARSEHASH_FLAGS = -I$(SPARSEHASH_INCL)

GTEST_DIR = googletest
GTEST_INC = -I$(GTEST_DIR)/include -I$(GTEST_DIR)
GTEST_FLAGS = -DGTEST_USE_OWN_TR1_TUPLE=0 -DGTEST_HAS_TR1_TUPLE=0 \
			  -DGTEST_LANG_CXX11=1 -Wno-narrowing $(GTEST_INC)

MAKEDEPEND = $(CXX) $(CXX11_FLAGS) $(GTEST_FLAGS) $(SPARSEHASH_FLAGS) \
			 -E -MM $(SRCFILES) $(TEST_SRCFILES) > depend.mk
#-----------------------------------------------------------------------------
# Source files.
#-----------------------------------------------------------------------------

SRCFILES =\
util.cpp\
board.cpp\
step.cpp\
gamestate.cpp\
arimaa_archive.cpp\
zobrist.cpp\
transposition.cpp\
move.cpp\
evaluate.cpp\
search.cpp\

OBJS = ${SRCFILES:.cpp=.o}
HEADERS = ${SRCFILES:.cpp=.h}

TEST_SRCFILES =\
test_board.cpp\
test_step.cpp\
test_gamestate.cpp\
test_arimaa_archive.cpp\
test_transposition.cpp\
test_move.cpp\
test_evaluate.cpp\
test_search.cpp\
test_util.cpp

TEST_OBJS = ${TEST_SRCFILES:.cpp=.o}

#-----------------------------------------------------------------------------
# Rules
#-----------------------------------------------------------------------------

test: test_all.x
	./test_all.x

test_all.x : $(OBJS) $(TEST_OBJS) libgtest.a
	$(CXX) $(LDFLAGS) $(GTEST_FLAGS) $^ -o $@

$(TEST_OBJS) : test_%.o : test_%.cpp
	$(CXX) $(CXXFLAGS) $(GTEST_FLAGS) -c test_$*.cpp -o $@

$(OBJS) : %.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $*.cpp

move.o : CXXFLAGS += $(SPARSEHASH_FLAGS)

libgtest.a:
	$(CXX) $(CXXFLAGS) $(GTEST_FLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) $(GTEST_FLAGS) -c $(GTEST_DIR)/src/gtest_main.cc
	$(AR) -rv libgtest.a gtest-all.o gtest_main.o

zobrist_array.include : transposition.cpp
	python zobrist_include.py 768 > $@
	
clean:
	-rm -r build *.so *.o *.pyc *.x

cleanall: clean
	-rm libgtest.a depend.mk

depend: depend.mk

depend.mk:
	$(MAKEDEPEND)

-include depend.mk
