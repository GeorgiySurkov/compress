CC=g++
CFLAGS=-c -pedantic-errors -std=c++20
LDFLAGS=
SOURCES=compress.cpp algorithms.cpp lib/cstring.cpp lib/string.cpp algorithms/Huffman.cpp algorithms/LZW.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=compress

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf **/*.o *.o $(EXECUTABLE)