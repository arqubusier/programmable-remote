PROG=test-main

INCLUDE =-I../src/ -I../Catch2/single_include/catch2/
INCLUDE += -I../etl/include
CXXFLAGS = -Wall -Wextra -g  -std=c++17

DEPS = ../src/util.hpp ../src/statemachine.hpp ../src/util_libopencm3.hpp

$(PROG): $(PROG).cxx $(DEPS)
	g++ $(CXXFLAGS) $(INCLUDE) -DTEST -o $@ $^
