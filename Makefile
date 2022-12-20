.PHONY: tests

all:
	g++ --std=c++14 -Wall -pedantic main.cpp memory/memory.cpp flags/flags.cpp executor/executor.cpp

tests:
	g++ --std=c++14 -o tests.out tests/executor_test.cpp executor/executor.cpp flags/flags.cpp
