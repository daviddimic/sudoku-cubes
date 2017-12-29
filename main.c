#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

#include "sudoku_cube.h"
#include "keyboard.h"


/* makro koji prverava da li je X izmedju A i B*/
#define IN(X, A, B) (((X) >= (A)) && ((X) <= (B)))

#define EPS 0.2

/* gravitaciona konstanta i broj PI */
#define G 9.81
#define pi 3.141592654

/*TODO rand menjati*/
static const float v0 = 6;


/* tekuca tabla koja se resava */
extern int curr_table;
/* niz tabli */
extern T tables[NUM_TABLES];

/* broj pomoci za resavanje sudoku */
extern int help_number;

/* uglovi rotacije kocke */
extern float y_rotation, x_rotation;

/* tajmeri animacije */
static int timer_active_fly;

/* koordinate kamere */
static float camera_x, camera_y, camera_z;
/* priblizavanje i udaljavanje pogleda na kocku */
extern double zoomInOut;

/* vreme animacije */
static float t;
/* parametri kretanja */
static float x_t, y_t;


/* OpenGL inicijalizacija */
static void initialize(void);

/* deklaracije callback funkcija */
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
    curr_table = FRONT;
    help_number = 2;
}


/* TODO izmeniti tajmer za kretanje kocke
static void on_timer_move(int value) {

    if(value != 1) return;

    t += 0.05;

    x_t = v0*t*t;
    y_t = -t*t/(v0-2);

    // da upadne u kocku
    if(IN(x_t - 7 - camera_x, -EPS, EPS) && IN(y_t + 1 - camera_y, - EPS, EPS)){
        timer_active_fly = 0;
    }

    glutPostRedisplay();

    if(timer_active_fly){
        glutTimerFunc(TIMER_ROTATE_WAIT, on_timer_move, 1);
    }
}
*/

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

    /* TODO kretanje kocke ka posmatracu i izbegavanje */
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
    
    char help_str[7] = "help: ";
    sprintf(help_str + strlen(help_str), "%d" , help_number);
    draw_text(help_str, 10, 25);


    glutSwapBuffers();
}
