PROGRAM = sudoku
CC      = gcc
CFLAGS  = -g -std=c99 -Wall -Wextra
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o sudoku_func.o sudoku_draw.o sudoku_cube.o keyboard.o timers.o display.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

sudoku_draw.o: sudoku_draw.c sudoku_draw.h
	$(CC) $(CFLAGS) -c $<

sudoku_func.o: sudoku_func.c sudoku_func.h
	$(CC) $(CFLAGS) -c $<

keyboard.o: keyboard.c keyboard.h
	$(CC) $(CFLAGS) -c $<

timers.o: timers.c timers.h
	$(CC) $(CFLAGS) -c $<

sudoku_cube.o: sudoku_cube.c sudoku_cube.h
	$(CC) $(CFLAGS) -c $<

display.o: display.c display.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM)

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
