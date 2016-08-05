GCC = g++ -g -Wall -pedantic -Wextra -std=c++14 -Wfatal-errors
PROG = array

all: test_Array.cpp Array.hpp
	$(GCC) -o $(PROG) test_Array.cpp Array.hpp

clean:
	rm $(PROG)
