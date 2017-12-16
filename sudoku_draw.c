#include <GL/glut.h>
#include "sudoku_draw.h"


void draw_table(const int table[][N], int indx, int indy, int border, double x, double y, double width, double r, double g, double b){
    draw_numbers(table, indx, indy, border, x, y, width, r, g, b);
    draw_grid(x, y, width);
}


void draw_grid(double x, double y, double width){

    /*okvir table*/
    glColor3f(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x + 0, y + 0);
        glVertex2f(x + width, y + 0);
        glVertex2f(x + width, y + width);
        glVertex2f(x + 0, y + width);
    glEnd();

    double width_cell = width/9.0;
    double height_cell = width/9.0;

    /*linije unutar table*/
    for(int i = 1; i < N; i++){
        glLineWidth(2);
        /*podebljana linija na svako trece*/
        if(i%3 == 0){
           glLineWidth(4);
       }
        glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(x + i*width_cell, y + 0);
            glVertex2f(x + i*width_cell, y + width);
        glEnd();

        glBegin(GL_LINES);
            glColor3f(0, 0, 0);
            glVertex2f(x + 0, y + i*height_cell);
            glVertex2f(x + width, y + i*height_cell);
        glEnd();
    }

    /*pozadnina table*/
    glColor3f(0.85, 0.85, 0.85);
    glBegin(GL_QUADS);
        glVertex2f(x + 0, y + 0);
        glVertex2f(x + width, y + 0);
        glVertex2f(x + width, y + width);
        glVertex2f(x + 0, y + width);
    glEnd();
}


/*crta crveni okvir tekuce pozicije na tabli*/
static void draw_border(double x, double y, double size){

    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
        glColor3f(1, 0, 0);
        glVertex2f(x , -y);
        glVertex2f(x + size, -y);
        glVertex2f(x + size, -y - size);
        glVertex2f(x , -y - size);
    glEnd();
}


void draw_numbers(const int table[][N], int indx, int indy, int border, double x, double y, double table_width, double r, double g, double b){
    /* sirina i visina pojedinacne celije za broj*/
    double width_cell = table_width/9.0;
    double height_cell = table_width/9.0;

    /* velicina broja */
    double size = height_cell/3.0;

    /* razmak unutar celije izmedju broja i ivica celije*/
    double spaceX = (width_cell - size)/2.0;
    double spaceY = (height_cell - 2*size)/2.0;

    /* pocetne vrednost iscrtavanja brojeva koje ce se menjati*/
    double x_curr = x + spaceX;
    double y_curr = (y + table_width) - height_cell + spaceY;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){

            /* crvenim se boji okvir u oznacenom polju*/
            if(border && i == indy && j == indx){
                draw_border(x_curr - spaceX, -y_curr - height_cell + spaceY, width_cell);
            }

            /* nule ne iscrtavamo*/
            if(table[i][j]) {
                glColor3f(r, g, b);
                draw_digit(table[i][j], x_curr, y_curr, size);
            }

            x_curr += width_cell;
        }

        x_curr = x + spaceX;
        y_curr -= height_cell;
    }
}


void draw_digit(const int number, double x, double y, double size){

    glLineWidth(150*size);

    switch(number) {
        case 0:
        glBegin(GL_LINE_LOOP);
            glVertex2f(x + 0, y + 0);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
            glVertex2f(x + 0, y + size);
        glEnd();
             break;

        case 1:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
        glEnd();
             break;

        case 2:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + 0, y + 0);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
        glEnd();
            break;

        case 3:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + 0, y + 0);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
        glEnd();
            break;

        case 4:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + size, y + size);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + 0, y + 2*size);
        glEnd();
            break;

        case 5:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x, y);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + 0, y + 2*size);
            glVertex2f(x + size, y + 2*size);
        glEnd();
            break;

        case 6:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + 0, y + 0);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + 0, y + size);
        glEnd();
            break;

        case 7:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
        glEnd();
            break;

        case 8:
        glBegin(GL_LINE_LOOP);
            glVertex2f(x + 0, y + 0);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
            glVertex2f(x + 0, y + size);
        glEnd();
            break;

        case 9:
        glBegin(GL_LINE_STRIP);
            glVertex2f(x + 0, y + 0);
            glVertex2f(x + size, y + 0);
            glVertex2f(x + size, y + size);
            glVertex2f(x + 0, y + size);
            glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + 2*size);
            glVertex2f(x + 0, y + 2*size);
            glVertex2f(x + 0, y + size);
        glEnd();
            break;
    }
}
