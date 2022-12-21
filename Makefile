.PHONY: tests

all:
	g++ --std=c++14 -Wall -pedantic main.cpp memory/memory.cpp flags/flags.cpp executor/executor.cpp executor/operations.cpp

tests:
	g++ --std=c++14 -o executor_test.out tests/executor_test.cpp executor/executor.cpp executor/operations.cpp flags/flags.cpp memory/memory.cpp
	g++ --std=c++14 -o memory_test.out tests/memory_test.cpp memory/memory.cpp
