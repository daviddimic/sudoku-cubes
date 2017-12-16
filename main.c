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


static float y_rotation, x_rotation;
static int timer_active;

/* priblizavanje i udaljavanje pogleda na kocku */
static double zoomInOut = 0;


/* OpenGL inicijalizacija */
static void initialize(void);

/* deklaracije callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_specialkeys(int key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

static void on_timer(int value);
static void on_timer2(int value);
static void on_timer3(int value);
static void on_timer4(int value);


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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);

    timer_active = 0;

    x_rotation = 0;
    y_rotation = 0;

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

static void set_timer(float *rotation, int *timer){
    float eps = 0.01;

    if(*rotation >= -eps && *rotation <= eps)
        *timer = 0;

    if(*rotation + 90 >= -eps && *rotation + 90 <= eps)
        *timer = 0;

    if(*rotation + 180 >= -eps && *rotation + 180 <= eps)
        *timer = 0;

    if(*rotation + 270 >= -eps && *rotation + 270 <= eps)
        *timer = 0;

    if(*rotation + 360 >= -eps && *rotation + 360 <= eps){
        *timer = 0;
        *rotation = 0;
    }

    if(*rotation - 90 >= -eps && *rotation - 90 <= eps)
        *timer = 0;

    if(*rotation - 180 >= -eps && *rotation - 180 <= eps)
        *timer = 0;

    if(*rotation - 270 >= -eps && *rotation - 270 <= eps)
        *timer = 0;

    if(*rotation - 360 >= -eps && *rotation - 360 <= eps){
        *timer = 0;
        *rotation = 0;
    }

}

static void on_timer3(int value){
    if (value != 0)
        return;

    y_rotation -= 10;
    set_timer(&y_rotation, &timer_active);

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(50, on_timer3, 0);
}

/*W minus*/
static void on_timer4(int value){
    if (value != 0)
        return;

    x_rotation -= 10;
    set_timer(&x_rotation, &timer_active);

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(50, on_timer4, 0);
}

static void on_timer2(int value){
    if (value != 0)
        return;

    x_rotation += 10;
    set_timer(&x_rotation, &timer_active);

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(50, on_timer2, 0);
}

static void on_timer(int value)
{
    if(value != 0)
        return;

    y_rotation += 10;

    set_timer(&y_rotation, &timer_active);

    glutPostRedisplay();

    if(timer_active)
        glutTimerFunc(50, on_timer, 0);
}

static void on_keyboard(unsigned char key, int x, int y) {
    /* izlaz iz programa ESC */
    switch (key) {
        case 27: exit(0);
        break;

        /* celu tablu vraca na pocetnu */
        case 'q':
            copy_tables(tables[curr_table].original, tables[curr_table].user, N);
        break;

        /* resi sudoku, moze se primeniti help_number broj puta */
        case 'r':
        if(help_number > 0){
            int a, b; /*NOTE: ne sluze nicemu, mora da se prosledi funkciji*/

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
        case 'd':
        if (!timer_active) {
            curr_table = next_table(key, curr_table);
            glutTimerFunc(50, on_timer3, 0);
            timer_active = 1;
        }
        break;

        case 'a':
        if (!timer_active) {
            curr_table = next_table(key, curr_table);
            glutTimerFunc(50, on_timer, 0);
            timer_active = 1;
        }
        break;

        case 'w':
        if (!timer_active) {
            curr_table = next_table(key, curr_table);
            glutTimerFunc(50, on_timer2, 0);
            timer_active = 1;
        }
        break;

        case 's':
        if (!timer_active) {
            curr_table = next_table(key, curr_table);
            glutTimerFunc(50, on_timer4, 0);
            timer_active = 1;
        }
        break;

        /* pomeranje kamere, zoomIn, zoomOut */
        case '+': zoomInOut = zoomInOut <= -3.5 ? zoomInOut : zoomInOut - 0.1;
        break;
        case '-': zoomInOut = zoomInOut >= 1.5 ? zoomInOut : zoomInOut + 0.1;
        break;

        /*TODO izbaciti rucno pomeranje*/
        case ' ': curr_table = (curr_table + 1) % NUM_TABLES;
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
        glRotatef(x_rotation, 1, 0, 0);
        glRotatef(y_rotation, 0, 1, 0);
        draw_cube(tables, NUM_TABLES, N, size, curr_table);
    glPopMatrix ();

    /* ako smo u kocki TODO opsti brojevi */
    if(zoomInOut >= -1.5 && zoomInOut <= -0.8){
        draw_text("Dude, GET OUT!", 400, 400);
    }

    char help_str[7] = "help: ";
    sprintf(help_str + strlen(help_str), "%d" , help_number);
    draw_text(help_str, 10, 25);


    glutSwapBuffers();
}
