CC = g++
CPP = $(wildcard *.cpp)
OBJ = $(CPP:.cpp=.o)

%.o: %.cpp
	$(CC) -c -o $@ $^

SimpleAStarExecutable: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o