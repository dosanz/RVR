CC=g++
CFLAGS=-g -I. -Wall
DEPS = Socket.h Game.h Tablero.h
OBJ = Socket.o Game.o Tablero.o
LIBS=-lpthread -lsfml-graphics -lsfml-window -lsfml-system -lX11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) Server.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) Client.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

