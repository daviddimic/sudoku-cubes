#include "sudoku_cube.h"
#include "sudoku_func.h"
#include "sudoku_draw.h"
#include <GL/glut.h>
#include <string.h>



void init_tables(T tables[NUM_TABLES], int n_tables, int n_table){
    for (int i = 0; i < n_tables; i++) {
        /*TODO da se ne ucitava nego generise*/
        read_from_file(tables[i].user, n_table, "sudoku.txt");
        read_from_file(tables[i].original, n_table, "sudoku.txt");
        tables[i].indx = 0;
        tables[i].indy = 0;
    }
}

void draw_cube(T tables[NUM_TABLES], int n_tables, int n_table, double size, int curr_table){
    /*FRONT*/
    glPushMatrix();
        draw_table(tables[FRONT].user, tables[FRONT].indx, tables[FRONT].indy, curr_table == FRONT,  0, 0, size);
    glPopMatrix();

    /*BACK*/
    glPushMatrix();
        glTranslatef(size, 0, -size);
        glRotatef(180, 0, 1, 0);
        draw_table(tables[BACK].user, tables[BACK].indx, tables[BACK].indy, curr_table == BACK, 0, 0, size);
    glPopMatrix();

    /*RIGHT*/
    glPushMatrix();
        glTranslatef(size, 0, 0);
        glRotatef(90, 0, 1, 0);
        draw_table(tables[RIGHT].user, tables[RIGHT].indx, tables[RIGHT].indy, curr_table == RIGHT, 0, 0, size);
    glPopMatrix();

    /*LEFT*/
    glPushMatrix();
        glTranslatef(0, 0, -size);
        glRotatef(-90, 0, 1, 0);
        draw_table(tables[LEFT].user, tables[LEFT].indx, tables[LEFT].indy, curr_table == LEFT, 0, 0, size);
    glPopMatrix();

    /*DOWN*/
    glPushMatrix();
        glTranslatef(0, 0, -size);
        glRotatef(90, 1, 0, 0);
        draw_table(tables[DOWN].user, tables[DOWN].indx, tables[DOWN].indy, curr_table == DOWN, 0, 0, size);
    glPopMatrix();

    /*UP*/
    glPushMatrix();
        glTranslatef(0, size, 0);
        glRotatef(-90, 1, 0, 0);
        draw_table(tables[UP].user, tables[UP].indx, tables[UP].indy, curr_table == UP, 0, 0, size);
    glPopMatrix();
}


void draw_text(const char* text, double x, double y){
    glColor3f(1, 0, 0);
    glMatrixMode(GL_PROJECTION);
    double matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 800, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        glLoadIdentity();
        glRasterPos2f(x,y);
        int len = strlen(text);
        for(int i = 0; i < len; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);//GLUT_BITMAP_9_BY_15,text);
        }
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}
