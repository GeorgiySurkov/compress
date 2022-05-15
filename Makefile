CC=g++
CFLAGS=-c -pedantic-errors -std=c++20
LDFLAGS=
SOURCES=compress.cpp algorithms.cpp lib/cstring.cpp algorithms/Huffman.cpp lib/string.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=compress

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf **/*.o *.o $(EXECUTABLE)