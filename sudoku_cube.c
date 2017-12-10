#include "sudoku_cube.h"
#include "sudoku_func.h"


void init_tables(T tables[NUM_TABLES], int n_tables, int n_table){
    for (int i = 0; i < n_tables; i++) {
        /*TODO da se ne ucitava nego generise*/
        read_from_file(tables[i].user, n_table, "sudoku.txt");
        read_from_file(tables[i].original, n_table, "sudoku.txt");
        tables[i].indx = 0;
        tables[i].indy = 0;
    }
}
