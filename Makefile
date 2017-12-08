PROGRAM = sudoku
CC      = gcc
CFLAGS  = -g -ansi -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o sudoku_func.o sudoku_draw.o
	$(CC) $(LDFLAGS) -o $(PROGRAM) main.o sudoku_func.o sudoku_draw.o $(LDLIBS)

sudoku_draw.o: sudoku_draw.c
	$(CC) -std=c99 -Wall -c sudoku_draw.c

sudoku_func.o:	sudoku_func.c
	$(CC) -std=c99 -Wall -c sudoku_func.c

main.o: main.c
	$(CC) -std=c99 -Wall -c main.c

.PHONY: beauty clean dist

beauty:
	-indent -nut -kr main.c
	-rm *~ *BAK

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
