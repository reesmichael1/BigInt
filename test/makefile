CC=g++
CXXFLAGS=-std=c++14 -Wall -pedantic

all: tests

test_skeleton.o: test_skeleton.cpp
	$(CC) $(CXXFLAGS) -c test_skeleton.cpp

tests: tests.cpp test_skeleton.o ../src/BigInt.h ../src/BigInt.cpp
	$(CC) $(CXXFLAGS) -o tests tests.cpp test_skeleton.o \
		../src/BigInt.cpp

clean:
	$(RM) tests
