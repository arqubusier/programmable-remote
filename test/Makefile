PROG=test-main

INCLUDE =-I../src/ -I../Catch2/single_include/catch2/
INCLUDE += -I../etl/include
$(PROG): $(PROG).cxx ../src/util.hpp
	g++ -Wall -Wextra -g $(INCLUDE) -DTEST -o $@ $^
