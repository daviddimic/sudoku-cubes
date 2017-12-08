#include "sudoku_draw.h"
#include <GL/glut.h>

void draw_table(double x, double y, double width){

    glColor3f(0, 0, 1);

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
    for(int i = 1; i<9; i++){
        glLineWidth(1);
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

}


/*TODO zavrsiti*/
void draw_numbers(int table[][9], int indx, int indy, double x, double y, double table_width){

    /*sirina i visina pojedinacne celije za broj*/
    double width_cell = table_width/9.0;
    double height_cell = table_width/9.0;

    /* velicina broja */
    double size = height_cell/3.0;

    /* razmak unutar celije izmedju broja i ivica celije*/
    double spaceX = (width_cell - size)/2.0;
    double spaceY = (height_cell - 2*size)/2.0;

    /*pocetne vrednost iscrtavanja brojeva*/
    x = x + spaceX;
    y = -y - height_cell + spaceY;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){

            /*crvenim se boji broj u oznacenom polju*/
            if(i == indy && j == indx){
                glColor3f(1,0,0);
                draw_digit(table[i][j], x, y, size);
            }
            else{
                glColor3f(0,0,0);
                draw_digit(table[i][j], x, y, size);
            }
            x += width_cell;
        }
        x = -1 + spaceX;
        y -= height_cell;
    }
}

void draw_digit(int number, double x, double y, double size){

    glLineWidth(50*size);

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
