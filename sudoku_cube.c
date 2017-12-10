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
    /*FRONT*/
    glPushMatrix();
        draw_table(tables[FRONT].user, tables[FRONT].indx, tables[FRONT].indy, 0, 0, size);
    glPopMatrix();

    /*BACK*/
    glPushMatrix();
        glTranslatef(size, 0, -size);
        glRotatef(180, 0, 1, 0);
        draw_table(tables[BACK].user, tables[BACK].indx, tables[BACK].indy, 0, 0, size);
    glPopMatrix();

    /*RIGHT*/
    glPushMatrix();
        glTranslatef(size, 0, 0);
        glRotatef(90, 0, 1, 0);
        draw_table(tables[RIGHT].user, tables[RIGHT].indx, tables[RIGHT].indy, 0, 0, size);
    glPopMatrix();

    /*LEFT*/
    glPushMatrix();
        glTranslatef(0, 0, -size);
        glRotatef(-90, 0, 1, 0);
        draw_table(tables[3].user, tables[3].indx, tables[3].indy, 0, 0, size);
    glPopMatrix();

    /*DOWN*/
    glPushMatrix();
        glTranslatef(0, 0, -size);
        glRotatef(90, 1, 0, 0);
        draw_table(tables[DOWN].user, tables[DOWN].indx, tables[DOWN].indy, 0, 0, size);
    glPopMatrix();

    /*UP*/
    glPushMatrix();
        glTranslatef(0, size, 0);
        glRotatef(-90, 1, 0, 0);
        draw_table(tables[UP].user, tables[UP].indx, tables[UP].indy, 0, 0, size);
    glPopMatrix();
}
