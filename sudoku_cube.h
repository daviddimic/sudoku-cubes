#ifndef SUDOKU_CUBE_H_
#define  SUDOKU_CUBE_H_

#include "sudoku_func.h"

/* broj tabli na kocki */
#define NUM_TABLES 6

enum position {
    FRONT,
    BACK,
    RIGHT,
    LEFT,
    DOWN,
    UP
};

/*  jedan sudoku mora cuvati tekuci koji korisnik resava i originalnu postavku
 *  kao i poziciju na tabli gde se upisuje broj
*/
typedef struct {
    /*korisnicka tabla*/
    int user[N][N];
    /*originalna postavka table*/
    int original[N][N];
    /*indeks pozicije na tekucoj tabli*/
    int indx, indy;
} T;

/* ucitava se sudoku kocka - niz tabli */
void init_tables(T tables[NUM_TABLES], int n_tables, int n_table);

/* crta sudoku kocku u (0,0,0) */
void draw_cube(T tables[NUM_TABLES], int n_tables, int n_table, double size);

#endif /*SUDOKU_CUBE_H_*/
