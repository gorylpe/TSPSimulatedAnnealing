CC=gcc
CFLAGS=-O3 -std=c99
LIBS=-lm
OBJS=main.o startingcycle.o swap2opt.o simulatedannealing.o util.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
clean:
	rm -f *.o
