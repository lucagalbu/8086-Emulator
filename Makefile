.PHONY: tests

all:
	g++ --std=c++14 main.cpp flags/flags.cpp executor/executor.cpp

tests:
	g++ --std=c++14 -o tests.out tests/executor_test.cpp executor/executor.cpp flags/flags.cpp