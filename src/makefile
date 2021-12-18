CC = em++
CPP = $(wildcard *.cpp)
OBJ = $(CPP:.cpp=.o)

# -s USE_BOOST_HEADERS=1
%.o: %.cpp
	$(CC) -c -o $@ $^

wasm_SimpleAStarExecutable.js: $(OBJ)
	$(CC) -o $@ $^


.PHONY: clean

clean:
	rm -f *.o
