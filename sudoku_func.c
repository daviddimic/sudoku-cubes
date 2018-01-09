#include "sudoku_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0

void error(const char* msg){
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void copy_tables(const int src[][N], int dest[][N], const int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void print_sudoku(const int tabla[][N], const int n){
    int i, j;
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++)
            if (j && j % 3 == 0)
                printf("| %c ", tabla[i][j] ? tabla[i][j] + '0' : '-');
            else
                printf(" %c ", tabla[i][j] ? tabla[i][j] + '0' : '-');

        if (i != n-1 && i % 3 == 2)
            printf("\n-----------------------------\n");
        else
            printf("\n");
    }
    printf("\n");
}

void read_from_file(int tabla[][N], int n, const char* datPath){
    FILE* dat = fopen(datPath, "r");
    if(dat == NULL)
        error("Datoteka gde se nalazi sudoku nije nadjena");

    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            fscanf(dat, "%d", &tabla[i][j]);

    fclose(dat);
}



bool is_conflict(const int tabla[][N], const int n, const int elem, const int vrsta, const int kolona){
    int i, j;

    //is_conflict vrste
    for (i = 0; i < n; i++)
        if(tabla[vrsta][i] == elem)
            return true;

    //is_conflict kolone
    for (i = 0; i < n; i++)
        if(tabla[i][kolona] == elem)
            return true;

    //is_conflict bloka
    int blok = 3;
    int pi = blok * (vrsta / blok);  //pocetak vrste u bloku
    int pj = blok * (kolona / blok); //pocetak kolone u bloku
    for (i = pi; i < pi + blok; i++)
        for (j = pj; j < pj + blok; j++)
            if(tabla[i][j] == elem)
                return true;

    return false;
}

bool is_table_empty(const int tabla[][N], const int n, int* vrsta, int* kolona){
    for (*vrsta = 0; *vrsta < n; (*vrsta)++)
        for (*kolona = 0; *kolona < n; (*kolona)++)
            if(tabla[*vrsta][*kolona] == 0)
                return true;
    return false;
}

bool solve_sudoku(int tabla[][N], const int n){
    int i, j, broj;

    if(!is_table_empty(tabla, n, &i, &j))
        return true;

    for(broj = 1; broj <= 9; broj++){ //probamo sve brojeve od 1 do 9
        //ako mozemo da stavimo broj
        if (!is_conflict(tabla, n, broj, i, j)){
            //stavljamo broj
            tabla[i][j] = broj;

            //rekurzivni poziv  za ostatak
            if (solve_sudoku(tabla, n))
                return true;

            //ako ne vodi ka resenju - BACKTRACK, vracamo na 0
            tabla[i][j] = 0;
        }
    }
    return false;
}


/* pocetak preuzetog koda sa: https://github.com/honzabrecka/sudoku-c
 * f-je koje generisu Sudoku
 *
 *  Copyright 2014 Jan Břečka
 *  This program is free software. You can redistribute and/or modify it
 *  in accordance with the terms of the accompanying license agreement.
 */
static int getAt(int * grid, int x, int y)
{
    int index = y * N + x;
    int value = *(grid + index);
    return value;
}

static int findEmptyIndex(int * grid)
{
    int length = N * N;
    int i;

    for (i = 0; i < length; i++) {
        if (*(grid + i) == EMPTY) {
            return i;
        }
    }

    return -1;
}

static int isPossible(int * grid, int index, int value)
{
    int boxN = N / 3;
    int x = index % N;
    int y = index / N;
    int boxX = x - (x % boxN);
    int boxY = y - (y % boxN);
    int i;

    for (i = 0; i < N; i++) {
        if (getAt(grid, i, y) == value ||
        getAt(grid, x, i) == value ||
        getAt(grid, boxX + (int)(i % boxN), boxY + (int)(i / boxN)) == value) {
            return 0;
        }
    }

    return 1;
}

static void swap(int * a, int * b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

static void shuffle(int * array, int length)
{
    int i, r;

    for (i = length - 1; i > 0; i--) {
        r = rand() % (i + 1);
        swap(&array[i], &array[r]);
    }
}

static void fill(int * array, int length, int start)
{
    int i;

    for (i = 0; i < length; i++) {
        array[i] = start + i;
    }
}

static int sudoku_generate(int * grid)
{
    int index = findEmptyIndex(grid);
    int values[N], i, value;

    if (index == -1) {
        return 1;
    }

    fill(values, N, 1);
    shuffle(values, N);

    for (i = 0; i < N; i++) {
        value = *(values + i);

        if (isPossible(grid, index, value)) {
            grid[index] = value;

            if (sudoku_generate(grid)) {
                return 1;
            }
            grid[index] = EMPTY;
        }
    }

    return 0;
}

static int sudoku_classic(int * grid, int empty)
{
    int length = N * N;
    int *indicesToRemove = (int *) malloc(length * sizeof(int));
    int i;

    fill(indicesToRemove, length, 0);
    shuffle(indicesToRemove, length);

    for (i = 0; i < empty; i++) {
        grid[*(indicesToRemove + i)] = EMPTY;
    }

    free(indicesToRemove);
    return 1;
}
/*kraj preuzetog koda*/


/* generisanje random sudokua - prilagodjena za matricu
 * num_empty je broj broj praznina u generisanoj tabli - tezina */
void generate_rand_sudoku(int table[][N], const int n, const int num_empty){
    /* preuzeta fja radi sa nizom sa 81 clanom
       koji mora biti inicijalizovan na 0 */
    int pom_table[N*N] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

    /* generise u potpunosti popunjen validni sudoku (koji ima jedinstveno resenje) nad nizom */
	sudoku_generate(pom_table);
    /* pravi random 'rupe' u generisanoj tabli kojih ima num_empty
     * stavlja num_empty broj nula na random mesta */
	sudoku_classic(pom_table, num_empty);

    /* u ostatku mog koda se radi sa matricom 9x9
     * pa se niz od 81 clan kopira u matricu i sa njom se dalje radi */
	for(int i=0; i<n*n; i++){
		table[i/N][i%N] = pom_table[i];
	}
}
