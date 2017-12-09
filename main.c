#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "sudoku_func.h"
#include "sudoku_draw.h"

static int window_width, window_height;

/*broj koji se unosi sa tastature*/
static unsigned number;

/*tekuca tabla koja se resava*/
static int curr_table = 0;

/*TODO upakovati u .h fajl*/
/* broj tabli na kocki */
#define NUM_TABLES 6

enum position {
    LEFT,
    FRONT,
    RIGHT,
    BACK,
    UP,
    DOWN
};

typedef struct {
    /*korisnicka tabla*/
    int user_table[N][N];
    /*originalna postavka table*/
    int org_table[N][N];
    /*indeks pozicije na tekucoj tabli*/
    int indx, indy;
} T;

static T tabla, tabla1;

static T tables[NUM_TABLES];


/*broj pomoci za resavanje sudoku*/
static int help_number = 2;


/* Funkcija initalize vrsi OpenGL inicijalizaciju. */
static void initialize(void);

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_specialkeys(int key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sudoku kocka");

    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(on_specialkeys);
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
    read_from_file(tabla.user_table, N, "sudoku.txt");
    read_from_file(tabla.org_table, N, "sudoku.txt");
    tabla.indx = 0;
    tabla.indy = 0;
    read_from_file(tabla1.user_table, N, "sudoku1.txt");
    read_from_file(tabla1.org_table, N, "sudoku1.txt");
    tabla1.indx = 0;
    tabla1.indy = 0;

    copy_tables(tabla.user_table, tables[0].user_table , N);
    copy_tables(tabla.org_table, tables[0].org_table , N);
    tables[0].indx = tabla.indx;
    tables[0].indy = tabla.indy;

    copy_tables(tabla1.user_table, tables[1].user_table , N);
    copy_tables(tabla1.org_table, tables[1].org_table , N);
    tables[1].indx = tabla1.indx;
    tables[1].indy = tabla1.indy;
}

static void on_specialkeys(int key, int x, int y){
    /* sa strelicama krece se kroz tekucu sudoku tablu */
    switch (key) {
        case GLUT_KEY_UP:
            tables[curr_table].indy = tables[curr_table].indy >= N || tables[curr_table].indy <= 0 ? 0 : tables[curr_table].indy-1;
            break;
        case GLUT_KEY_DOWN:
            tables[curr_table].indy = tables[curr_table].indy >= N-1 ? N-1 : tables[curr_table].indy+1;
            break;
        case GLUT_KEY_LEFT:
            tables[curr_table].indx = tables[curr_table].indx <= 0 ? 0 : tables[curr_table].indx-1;
            break;
        case GLUT_KEY_RIGHT:
            tables[curr_table].indx = tables[curr_table].indx >= N-1 ? N-1 : tables[curr_table].indx+1;
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

        /* na tekucem polju vraca stara vrednost iz originalne table */
        case '-':
            tables[curr_table].user_table[tables[curr_table].indy][tables[curr_table].indx] = tables[curr_table].org_table[tables[curr_table].indy][tables[curr_table].indx];
            break;

        /* celu tablu vraca na pocetnu */
        case 'q':
            copy_tables(tables[curr_table].org_table, tables[curr_table].user_table, N);
            break;

        /* resi sudoku, moze se primeniti help_number broj puta */
        case 'r':
            if(help_number > 0){
                if(solve_sudoku(tables[curr_table].org_table, N) == true){
                    copy_tables(tables[curr_table].org_table, tables[curr_table].user_table, N);
                    help_number--;
                }
                else {
                    error("Sudoku nema resenja!");
                }
            }
            break;

        /* krecemo se kroz razlicite sudoku table */
        case ' ':
            curr_table = (curr_table + 1)%NUM_TABLES;
            break;

        /* unos brojeva */
        case '0': case '1': case '2':
        case '3': case '4': case '5':
        case '6': case '7': case '8':
        case '9':
                number = key - '0';
                if(!(number && is_conflict(tables[curr_table].user_table, N, number, tables[curr_table].indy, tables[curr_table].indx))){
                    /*ne moze se broj upisati na to mesto*/
                    tables[curr_table].user_table[tables[curr_table].indy][tables[curr_table].indx] = number;
                }
            break;
    }

    glutPostRedisplay();
}

static void draw_table(int table[][N], int indx, int indy, double x, double y, double width){
    draw_grid(x, y, width);
    draw_numbers(table, indx, indy, x, y, width);
}

static void on_display(void) {
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window_width, window_height);

    glLoadIdentity();
/*
    glScalef(0.8, 0.8, 1);
*/
    double start = -1, size = 0.8;

    /*prva tabla*/
    draw_table(tables[1].user_table, tables[1].indx, tables[1].indy, start, start, size);

    /*druga tabla*/
    draw_table(tables[0].user_table, tables[0].indx, tables[0].indy, start + size + 0.2, start, size);

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
