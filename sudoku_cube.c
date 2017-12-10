#include "sudoku_cube.h"
#include "sudoku_func.h"
#include "sudoku_draw.h"
#include <GL/glut.h>



void init_tables(T tables[NUM_TABLES], int n_tables, int n_table){
    for (int i = 0; i < n_tables; i++) {
        /*TODO da se ne ucitava nego generise*/
        read_from_file(tables[i].user, n_table, "sudoku.txt");
        read_from_file(tables[i].original, n_table, "sudoku.txt");
        tables[i].indx = 0;
        tables[i].indy = 0;
    }
}

void draw_cube(T tables[NUM_TABLES], int n_tables, int n_table, double size){
    glPushMatrix();
        draw_table(tables[0].user, tables[0].indx, tables[0].indy, 0, 0, size);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(size, 0, -size);
        glRotatef(180, 0, 1, 0);
        draw_table(tables[1].user, tables[1].indx, tables[1].indy, 0, 0, size);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(size, 0, 0);
        glRotatef(90, 0, 1, 0);
        draw_table(tables[2].user, tables[2].indx, tables[2].indy, 0, 0, size);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, -size);
        glRotatef(-90, 0, 1, 0);
        draw_table(tables[3].user, tables[3].indx, tables[3].indy, 0, 0, size);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 0, 0);
        glRotatef(-90, 1, 0, 0);
        draw_table(tables[4].user, tables[4].indx, tables[4].indy, 0, 0, size);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, size, 0);
        glRotatef(-90, 1, 0, 0);
        draw_table(tables[5].user, tables[5].indx, tables[5].indy, 0, 0, size);
    glPopMatrix();
}
