CC=g++
CXXFLAGS=-std=c++11 -Wall -pedantic

all: tests

test_skeleton.o: test_skeleton.cpp
	$(CC) $(CXXFLAGS) -c test_skeleton.cpp

tests: tests.cpp test_skeleton.o ../src/BigInt.h ../src/BigInt.cpp
	# tests.exe output for Windows compatability
	$(CC) $(CXXFLAGS) -o tests.exe tests.cpp test_skeleton.o \
		../src/BigInt.cpp

clean:
	$(RM) tests.exe
