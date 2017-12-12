#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "sudoku_draw.h"
#include "sudoku_cube.h"
#include "sudoku_func.h"

static int window_width, window_height;

/* broj koji se unosi sa tastature */
static unsigned number;

/* tekuca tabla koja se resava */
static int curr_table = FRONT;

/* niz tabli */
static T tables[NUM_TABLES];

/* broj pomoci za resavanje sudoku */
static int help_number = 2;


static double xAngle = 0;
static double yAngle = 0;
static double zAngle = 0;

static double zoomInOut = 0;


/* OpenGL inicijalizacija */
static void initialize(void);

/* deklaracije callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_specialkeys(int key, int x, int y);
/* TODO unos cifara
static void on_mouseclick(int button, int state, int x, int y);
*/
static void on_reshape(int width, int height);
static void on_display(void);


/*
static enum position next_table(unsigned char key, enum position curr_table){
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
                case 'a': return LEFT;
                case 'd': return RIGHT;
            }
        //TODO
        default: return FRONT;
    }
}
*/


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sudoku kocka");

    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(on_specialkeys);
    /*
    glutMouseFunc(on_mouseclick);
    */
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    initialize();

    glutMainLoop();

    return 0;
}

static void on_reshape(int width, int height) {
    window_width = width;
    window_height = height;
}

static void initialize(void) {
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);

    init_tables(tables, NUM_TABLES, N);
}

static void on_specialkeys(int key, int x, int y){
    /* sa strelicama krece se kroz tekucu sudoku tablu */
    switch (key) {
    case GLUT_KEY_UP:
        tables[curr_table].indy = (tables[curr_table].indy >= N || tables[curr_table].indy <= 0) ? 0 : tables[curr_table].indy-1;
        break;
    case GLUT_KEY_DOWN:
        tables[curr_table].indy = (tables[curr_table].indy >= N-1) ? N-1 : tables[curr_table].indy+1;
        break;
    case GLUT_KEY_LEFT:
        tables[curr_table].indx = (tables[curr_table].indx <= 0) ? 0 : tables[curr_table].indx-1;
        break;
    case GLUT_KEY_RIGHT:
        tables[curr_table].indx = (tables[curr_table].indx >= N-1) ? N-1 : tables[curr_table].indx+1;
        break;
    }
    glutPostRedisplay();
}


static void on_keyboard(unsigned char key, int x, int y) {
    /* izlaz iz programa ESC */
    switch (key) {
    case 27:
        exit(0);
        break;

    /* celu tablu vraca na pocetnu */
    case 'q':
        copy_tables(tables[curr_table].original, tables[curr_table].user, N);
        break;

    /* resi sudoku, moze se primeniti help_number broj puta */
    case 'r':
        if(help_number > 0){
            int a, b; /*ne sluze nicemu, mora da se prosledi funkciji*/
            /* ako je tabla popunjena onda je sudoku vec resen */
            if(is_table_empty(tables[curr_table].original, N, &a, &b)){
                /* resavamo sudoku i umanjujemo broj pomoci */
                if(solve_sudoku(tables[curr_table].original, N)) {
                    copy_tables(tables[curr_table].original, tables[curr_table].user, N);
                    help_number--;
                }
                else {
                    error("Sudoku nema resenja!");
                }
            }
        }
        break;

    /*  krecemo se kroz razlicite sudoku table
     *  okrecemo kocku na wsad
        TODO
     */
    case 'w':
        xAngle += 10;
        break;
    case 's':
        xAngle -= 10;
        break;
    case 'a':
        yAngle += 10;
        break;
    case 'd':
        yAngle -= 10;
        break;

    /* pomeranje kamere, zoomIn, zoomOut */
    case '+':
        zoomInOut = zoomInOut <= -3.5 ? zoomInOut : zoomInOut - 0.1;
        break;
    case '-':
        zoomInOut = zoomInOut >= 1.5 ? zoomInOut : zoomInOut + 0.1;
        break;

    case ' ':
        curr_table = (curr_table + 1) % NUM_TABLES;
        break;

    /* unos brojeva u tekucu tablu */
    case '0': case '1': case '2':
    case '3': case '4': case '5':
    case '6': case '7': case '8':
    case '9':
        number = key - '0';
        /* ne moze se promeniti polje iz originalne postavke */
        if(!tables[curr_table].original[tables[curr_table].indy][tables[curr_table].indx]){
            /* ne moze se upisati broj osim nule ako dodje do konflikta brojeva */
            if(!(number && is_conflict(tables[curr_table].user, N, number, tables[curr_table].indy, tables[curr_table].indx))){
                tables[curr_table].user[tables[curr_table].indy][tables[curr_table].indx] = number;
            }
        }

        break;
    }
    glutPostRedisplay();
}




static void on_display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            0.2, 5);



    /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            1 + zoomInOut, 0.8 + zoomInOut, 1.5 + zoomInOut,
            0, 0, 0,
            0, 1, 0);

    /* velicina kocke */
    double size = 1;


    glPushMatrix ();
      glRotatef (zAngle, 0, 0, 1);
      glRotatef (yAngle, 0, 1, 0);
      glRotatef (xAngle, 1, 0, 0);
      /*pomeranje u (0, 0, 0)*/
      glTranslatef(-size/2.0, -size/2, size/2);
      draw_cube(tables, NUM_TABLES, N, size, curr_table);
    glPopMatrix ();

    if(zoomInOut >= -1.5 && zoomInOut <= -0.8){
        draw_text("Dude, GET OUT!", 400, 400);
    }

    char help_str[7] = "help: ";
    sprintf(help_str + strlen(help_str), "%d" , help_number);
    draw_text(help_str, 10, 25);


    glutSwapBuffers();
}
