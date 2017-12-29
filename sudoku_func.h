/*
* Osnovne funkcije za rad i resavanje jedne sudoku table
* predstavljene kao matrica 9x9
*/

#ifndef SUDOKU_FUNC_H_
#define  SUDOKU_FUNC_H_

#include <stdbool.h>

/*dimenzija sudoku table*/
#define N 9

/* ispis greske i izlaz iz programa */
void error(const char* msg);

/*kopira sadrzaj table src u dest*/
void copy_tables(const int src[][N], int dest[][N], const int n);

/*ispisuje sudoku u konzoli na standardni izlaz*/
void print_sudoku(const int tabla[][N], const int n);

/*ucitava sudoku iz datoteke*/
void read_from_file(int tabla[][N], int n, const char* datPath);

/*generise sudoku sa brojem praznih mesta num_empty */
void generate_rand_sudoku(int table[][N], const int n, const int num_empty);

/*vraca true ako na vrsti, koloni ili bloku 3x3 dodje do konflikta*/
bool is_conflict(const int tabla[][N], const int n, const int elem, const int i, const int j);

/*vraca true ako postoji nepopunjeno polje i vraca njegove indekse*/
bool is_table_empty(const int tabla[][N], const int n, int* vrsta, int* kolona);

/*glavna backtracking f-ja koja resava sudoku*/
bool solve_sudoku(int tabla[][N], const int n);


#endif /* SUDOKU_FUNC_H_ */
