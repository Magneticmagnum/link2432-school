# $Id: Exp $
# makefile for 3081 lab 6 Fall 2010

CPP = g++ -fprofile-arcs -ftest-coverage
CXX_DIR = ../cxxtest
CXX = $(CXX_DIR)/cxxtestgen.pl

.SUFFIXES : .o .cpp .c
.cpp.o :
	$(CPP) -c -g $<

TESTS = \
	StackTests.h

OBJS = \
	Stack.o


all: run_tests

Stack.o: Stack.h Stack.cpp


# Run the unit tests
run_tests: runner
	./runner
	gcov Stack.cpp

# Build the test runner
runner: runner.cpp $(OBJS)
	$(CPP) -I$(CXX_DIR) -o runner runner.cpp Stack.o

# Generate the test runner
runner.cpp: $(TESTS)
	$(CXX) --error-printer -o runner.cpp StackTests.h

# Generate documentation
doc: Doxyfile $(OBJS)
	doxygen

# Clean up
clean: 
	rm runner* *.o


