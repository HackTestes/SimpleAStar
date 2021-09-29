CC=g++
CPP= FnFunctions.cpp FunctionsUtils.cpp Node.cpp simple_Astar.cpp FunctionsConfig.cpp
HEADERS= AStarHeader.h json.hpp
OBJ= FnFunctions.o FunctionsUtils.o Node.o simple_Astar.o FunctionsConfig.o

%.o: %.cpp
	$(CC) -c -o $@ $^ -std=c++17

SimpleAStarExecutable: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o