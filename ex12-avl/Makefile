CC=g++
CFLAGS=-c -Wall -g -std=c++11
LDFLAGS=
SOURCES=avl.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main.out

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@
