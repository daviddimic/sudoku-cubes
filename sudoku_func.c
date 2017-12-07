#include "sudoku_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
