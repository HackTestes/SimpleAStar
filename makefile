CC=g++
CPP= FnFunctions.cpp FunctionsUtils.cpp Node.cpp simple_Astar.cpp
HEADERS= AStarHeader.h
OBJ= FnFunctions.o FunctionsUtils.o Node.o simple_Astar.o

%.o: %.cpp %.h
	$(CC) -c -o $@ $(CPP)

SimpleAStarExecutabele: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o 