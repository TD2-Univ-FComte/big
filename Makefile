GTEST_DIR = ./googletest/googletest

CFLAGS = -Wall -Wextra -std=c99 -O2 -g -pedantic -fprofile-arcs -ftest-coverage
CXXFLAGS = -Wall -Wextra -std=c++11 -O2 -g -pedantic -I$(GTEST_DIR)/include -I$(GTEST_DIR)
LDFLAGS = -pthread -fprofile-arcs


TARGETS = testbig

all: testbig

clean:
	rm -f *.o $(TARGETS)
	rm -f *.gcda *.gcno *.gcov

testbig: big.o ${GTEST_DIR}/src/gtest-all.o testbig.cc
big.o: big.c big.h
