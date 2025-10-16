CC=g++
CFLAGS= -std=c++17 -pedantic -Wall -g -O3 -L/Users/fritz/local/lib
INCLUDE= -I/Users/fritz/mylib/include -I./include -I/Users/fritz/local/include
LIBRARY= -lboost_program_options
EXEDIR= ./bin

MAIN= main.cpp
EXE=contactmap

$(EXE): $(MAIN)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBRARY) -o $(addprefix $(EXEDIR)/, $(EXE)) $(MAIN)

clean:
	rm $(addprefix $(EXEDIR)/, $(EXE))
