#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <GL/glut.h>

#include "sudoku_draw.h"
#include "sudoku_cube.h"
#include "sudoku_func.h"

#define TIMER_ROTATE_WAIT 50
#define TIMER_ROTATE_ID 0

/* makro koji prverava da li je X izmedju A i B*/
#define IN(X, A, B) (((X) >= (A)) && ((X) <= (B)))

#define EPS 0.2

/* gravitaciona konstanta i broj PI */
#define G 9.81
#define pi 3.141592654

/*TODO rand menjati*/
static const float v0 = 6;


/* tekuca tabla koja se resava */
static int curr_table = FRONT;
/* niz tabli */
static T tables[NUM_TABLES];

/* broj pomoci za resavanje sudoku */
static int help_number = 2;

/* za rotiranje kocke */
static unsigned char wsad_key;
/* uglovi rotacije kocke */
static float y_rotation, x_rotation;

/* tajmeri animacije */
static int timer_rotate_active, timer_active_fly;

/* koordinate kamere */
static float camera_x, camera_y, camera_z;
/* priblizavanje i udaljavanje pogleda na kocku */
static double zoomInOut;

/* vreme animacije */
static float t;
/* parametri kretanja */
static float x_t, y_t;



/* OpenGL inicijalizacija */
static void initialize(void);

/* deklaracije callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_specialkeys(int key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

/* tajmer za okretanje kocke */
static void on_timer_wsad(int value);


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
    glViewport(0, 0, width, height);
    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 0.2, 50);
}

static void initialize(void) {
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);

    /* osvetljenje */
    GLfloat ambient[] = {.2, .2, .2, 1};
    GLfloat diffuse[] = {.6, .6, .6, 1};
    GLfloat specular[] = {.9, .9, .9, 1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_COLOR_MATERIAL);

    GLfloat ambient_coef[] = {.1, .1, .1, 1};
    GLfloat diffuse_coef[] = {.6, .6, .6, 1};
    GLfloat specular_coef[] = {.9, .9, .9, 1};

    GLint shininess = 30;
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coef);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coef);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coef);
    glMateriali(GL_FRONT, GL_SHININESS, shininess);

    timer_rotate_active = 0;
    timer_active_fly = 0;

    t = 0;
    x_t = 0;
    y_t = 0;

    zoomInOut = 0;

    camera_x = 1;
    camera_y = 0.8;
    camera_z = 1.5;

    x_rotation = 0;
    y_rotation = 0;

    /* ucitavamo 6 tabli */
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

/* za koliko treba rotirati kocku i kada zaustaviti tajmer */
static void set_rotation_and_timer(float *rotation, int *timer){
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


static void on_timer_wsad(int value) {

    if(value != TIMER_ROTATE_ID) return;

    assert(wsad_key == 'w' || wsad_key == 's' || wsad_key == 'a' || wsad_key == 'd');

    switch (wsad_key) {
        case 'w':
            /* TODO ne moze se ici dalje ako smo na gornjoj tabli */
            x_rotation += 10;
            set_rotation_and_timer(&x_rotation, &timer_rotate_active);
        break;

        case 's':
            x_rotation -= 10;
            set_rotation_and_timer(&x_rotation, &timer_rotate_active);
        break;

        case 'a':
            y_rotation += 10;
            set_rotation_and_timer(&y_rotation, &timer_rotate_active);
        break;

        case 'd':
            y_rotation -= 10;
            set_rotation_and_timer(&y_rotation, &timer_rotate_active);
        break;
    }

    glutPostRedisplay();

    if(timer_rotate_active){
        glutTimerFunc(TIMER_ROTATE_WAIT, on_timer_wsad, TIMER_ROTATE_ID);
    }
}


/* TODO */
static void on_timer_fly(int value) {

    if(value != 1) return;

    t += 0.05;

    x_t = v0*t*t;
    y_t = -t*t/(v0-2);

    /*da upadne u kocku*/
    //printf("%f %f\n",x_t - 7 - camera_x, y_t + 1 - camera_y);
    if(IN(x_t - 7 - camera_x, -EPS, EPS) && IN(y_t + 1 - camera_y, -EPS, EPS)){
        timer_active_fly = 0;
    }

    glutPostRedisplay();

    if(timer_active_fly){
        glutTimerFunc(TIMER_ROTATE_WAIT, on_timer_fly, 1);
    }
}

static void on_keyboard(unsigned char key, int x, int y) {
    /*broj koji se unosi sa tastature*/
    unsigned number;

    switch (key) {
        /* izlaz iz programa ESC */
        case 27: exit(0);

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

        /* krecemo se kroz razlicite sudoku table
        *  okrecemo kocku na wsad */
        case 'w':
        case 's':
        case 'a':
        case 'd':
            /* ne moze se okretati kocka ako se krece */
            if (!timer_active_fly && !timer_rotate_active) {
                wsad_key = key;
                curr_table = next_table(key, curr_table);
                glutTimerFunc(TIMER_ROTATE_WAIT, on_timer_wsad, TIMER_ROTATE_ID);
                timer_rotate_active = 1;
            }
        break;

        /* pomeranje kamere, zoomIn, zoomOut */
        case '+': zoomInOut = zoomInOut <= -3.5 ? zoomInOut : zoomInOut - 0.1; break;
        case '-': zoomInOut = zoomInOut >= 1.5 ? zoomInOut : zoomInOut + 0.1;  break;

        /* TODO pokretanje kretanaj kocke */
        case ' ':
            if (!timer_active_fly) {
                glutTimerFunc(20, on_timer_fly, 1);
                timer_active_fly = 1;
            }
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

    /* postavljanej pozicije svetla */
    GLfloat light_position[] = {-0.5, 0.5, 4, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glShadeModel(GL_SMOOTH);

    /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            camera_x + zoomInOut, camera_y + zoomInOut, camera_z,
            0, 0, 0,
            0, 1, 0);

    /* velicina kocke */
    double size = 1;


    /* kretanje kocke ka posmatracu */
/*
    glTranslatef(-5, 1, -5);
    glRotatef(-45, 0, 1, 0);
    glTranslatef(x_t, y_t, 0);
    glRotatef(60, 0, 1, 0);
*/
    glPushMatrix();
        /* rotacija kocke preko wsad */
        glRotatef(x_rotation, 1, 0, 0);
        glRotatef(y_rotation, 0, 1, 0);
        /* crta se kocka */
        draw_cube(tables, size, curr_table);
    glPopMatrix();


    /* ispis poruke o broju pomoci - resavanje jednog sudoku */
    glDisable(GL_LIGHTING);
    char help_str[7] = "help: ";
    sprintf(help_str + strlen(help_str), "%d" , help_number);
    draw_text(help_str, 10, 25);
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}
