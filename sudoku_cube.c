#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glut.h>
#include "sudoku_cube.h"
#include "sudoku_func.h"
#include "sudoku_draw.h"

/* boja cifara originalne postavke */
#define O_R .3
#define O_G .05
#define O_B .3

/* boja cifara koje korisnik unosi */
#define U_R 0.09
#define U_G 0.46
#define U_B 0.23

/* za ispis sata hh : mm : ss */
#define MAX_LEN 13

void init_tables(T tables[NUM_TABLES], int n_tables, int n_table, int* help_number, time_t* start_time){
    srand(time(NULL));
    for (int i = 0; i < n_tables; i++) {
        generate_rand_sudoku(tables[i].original, n_table, 48 - i);
        copy_tables(tables[i].original, tables[i].user, n_table);
        tables[i].indx = 0;
        tables[i].indy = 0;
    }
    *help_number = HELP_NUMBER;
    *start_time = time(NULL);
}

enum position next_table(unsigned char key, enum position curr_table){

    assert(key == 'w' || key == 's' || key == 'a' || key == 'd');

    switch (curr_table) {
        case FRONT:
            switch (key) {
                case 'w': return UP;
                case 's': return DOWN;
                case 'a': return LEFT;
                case 'd': return RIGHT;
            }

        case RIGHT:
            switch (key) {
                case 'w': return UP;
                case 's': return DOWN;
                case 'a': return FRONT;
                case 'd': return BACK;
            }

        case LEFT:
            switch (key) {
                case 'w': return UP;
                case 's': return DOWN;
                case 'a': return BACK;
                case 'd': return FRONT;
            }

        case UP:
            switch (key) {
                case 'w': return BACK;
                case 's': return FRONT;
                case 'a':
                case 'd': return UP;
            }

        case DOWN:
            switch (key) {
                case 'w': return FRONT;
                case 's': return BACK;
                case 'a':
                case 'd': return DOWN;
            }
        case BACK:
            switch (key) {
                case 'w': return UP;
                case 's': return DOWN;
                case 'a': return RIGHT;
                case 'd': return LEFT;
            }
        default: return FRONT;
    }
}


void draw_cube(T tables[NUM_TABLES], double size, int curr_table){
    /*FRONT*/
    glPushMatrix();
        glTranslatef(0, 0, size/2);
        draw_table(tables[FRONT].original, tables[FRONT].indx, tables[FRONT].indy, curr_table == FRONT, -size/2, -size/2, size, O_R, O_G, O_B);
        draw_table(tables[FRONT].user, tables[FRONT].indx, tables[FRONT].indy, curr_table == FRONT, -size/2, -size/2, size, U_R, U_G, U_B);
    glPopMatrix();

    /*BACK*/
    glPushMatrix();
        glTranslatef(size, 0, -size/2);
        glRotatef(180, 0, 1, 0);
        draw_table(tables[BACK].original, tables[BACK].indx, tables[BACK].indy, curr_table == BACK, size/2, -size/2, size, O_R, O_G, O_B);
        draw_table(tables[BACK].user, tables[BACK].indx, tables[BACK].indy, curr_table == BACK, size/2, -size/2, size, U_R, U_G, U_B);
    glPopMatrix();

    /*RIGHT*/
    glPushMatrix();
        glTranslatef(size/2, 0, 0);
        glRotatef(90, 0, 1, 0);
        draw_table(tables[RIGHT].original, tables[RIGHT].indx, tables[RIGHT].indy, curr_table == RIGHT, -size/2, -size/2, size, O_R, O_G, O_B);
        draw_table(tables[RIGHT].user, tables[RIGHT].indx, tables[RIGHT].indy, curr_table == RIGHT, -size/2, -size/2, size, U_R, U_G, U_B);
    glPopMatrix();

    /*LEFT*/
    glPushMatrix();
        glTranslatef(-size/2, 0, 0);
        glRotatef(-90, 0, 1, 0);
        draw_table(tables[LEFT].original, tables[LEFT].indx, tables[LEFT].indy, curr_table == LEFT, -size/2, -size/2, size,  O_R, O_G, O_B);
        draw_table(tables[LEFT].user, tables[LEFT].indx, tables[LEFT].indy, curr_table == LEFT, -size/2, -size/2, size, U_R, U_G, U_B);
    glPopMatrix();

    /*DOWN*/
    glPushMatrix();
        glTranslatef(0, -size/2, 0);
        glRotatef(90, 1, 0, 0);
        draw_table(tables[DOWN].original, tables[DOWN].indx, tables[DOWN].indy, curr_table == DOWN, -size/2, -size/2, size, O_R, O_G, O_B);
        draw_table(tables[DOWN].user, tables[DOWN].indx, tables[DOWN].indy, curr_table == DOWN, -size/2, -size/2, size, U_R, U_G, U_B);
    glPopMatrix();

    /*UP*/
    glPushMatrix();
        glTranslatef(0, size/2, 0);
        glRotatef(-90, 1, 0, 0);
        draw_table(tables[UP].original, tables[UP].indx, tables[UP].indy, curr_table == UP, -size/2, -size/2, size, O_R, O_G, O_B);
        draw_table(tables[UP].user, tables[UP].indx, tables[UP].indy, curr_table == UP, -size/2, -size/2, size, U_R, U_G, U_B);
    glPopMatrix();
}


void draw_text(const char* text, double x, double y){
    glPushMatrix();

    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.8, 0.0);

    glMatrixMode(GL_PROJECTION);
    double matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 800, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLoadIdentity();
    glRasterPos2f(x,y);
    int len = strlen(text);
    for(int i = 0; i < len; i++){
        if(text[i] == '\n'){
            y -= 20;
            glRasterPos2f(x, y);
        }
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void draw_elapsed_time(time_t start_time){
    time_t current_time = time(NULL);

    time_t passed_time = difftime(current_time, start_time);
    struct tm *time_format = gmtime(&passed_time);
    char buff[MAX_LEN + 1];
    strftime(buff, MAX_LEN, "%H : %M : %S", time_format);

    draw_text(buff, 350, 780);
}

int is_cube_solved(T tables[NUM_TABLES], int n_tables, int n_table){
    /* kocka je resena ako je svaki sudoku resen (popunjen) */
    int is_solved = 1;
    /*NOTE a i b ne sluzi nicemu, mora se proslediti f-ji*/
    int a, b;
    for (int i = 0; i < n_tables; i++) {
        if(is_table_empty(tables[i].original, n_table, &a, &b) ||
           is_table_empty(tables[i].user, n_table, &a, &b)){
            is_solved = 0;
        }
    }
    return is_solved;
}
