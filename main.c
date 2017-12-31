#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

#include "sudoku_cube.h"
#include "keyboard.h"


/* tekuca tabla koja se resava */
extern int curr_table;
/* niz tabli */
extern T tables[NUM_TABLES];
/* broj pomoci za resavanje sudoku */
extern int help_number;

/* uglovi rotacije kocke */
extern float y_rotation, x_rotation;

/* ugao i translacija spinovanja kocke */
extern float spin_angle;
extern float spin_y;

/* koordinate kamere */
extern float camera_x, camera_y, camera_z;
/* priblizavanje i udaljavanje pogleda na kocku */
extern double zoomInOut;

/* parametri kretanja */
extern float x_t, y_t;


/* vreme pri pokretanju programa */
extern time_t start_time;


static const char* usage_str = " rotate cube: w-s-a-d\n\
navigate in table: arrow keys\n\
zoom in/out: + -\n\
input number: 1-9\n\
remove number: 0\n\
remove all numbers: q\n\
solve one sudoku: h\n\
new game: n";


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
    glClearColor(0.3, 0.3, 0.3, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

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

    x_t = 0;
    y_t = 0;

    zoomInOut = 0;

    camera_x = 1;
    camera_y = 0.8;
    camera_z = 1.5;

    x_rotation = 0;
    y_rotation = 0;

    /* ucitavamo 6 tabli */
    init_tables(tables, NUM_TABLES, N, &help_number, &start_time);
    /* pocetna tabla je prednja */
    curr_table = FRONT;
}


static void on_display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* postavljanej pozicije svetla */
    GLfloat light_position[] = {-0.5, 1, 4, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glShadeModel(GL_SMOOTH);

    /* podesava se tacka pogleda */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            camera_x + zoomInOut, camera_y + zoomInOut, camera_z,
            0, 0, 0,
            0, 1, 0);

    /* velicina kocke */
    double size = 1;

    /* TODO kretanje kocke ka posmatracu i izbegavanje
    glTranslatef(-5, 1, -5);
    glRotatef(-45, 0, 1, 0);
    glTranslatef(x_t, y_t, 0);
    glRotatef(60, 0, 1, 0);
    */

    /* ako je kocka resena ispisuje se poruka
     * i pitanje za dalju igru
     * kocka se privremeno udaljava
     */
    if(is_cube_solved(tables, NUM_TABLES, N)){
        draw_text("YOU WIN!", 350, 400);
        draw_text("new game? (yes: n / no: esc)", 270, 340);
        glTranslatef(0, -0.5, 0);
    }

    glPushMatrix();
        /* spinovanje kocke za novu igru */
        glTranslatef(0, spin_y, 0);
        glRotatef(-spin_angle, 1, 0, 0);

        /* rotacija kocke preko wsad */
        glRotatef(x_rotation, 1, 0, 0);
        glRotatef(y_rotation, 0, 1, 0);

        /* crta se kocka */
        draw_cube(tables, size, curr_table);
    glPopMatrix();


    /* poruka o koriscenju */
    draw_text(usage_str, 10, 780);

    /* ispis poruke o broju pomoci - resavanje jednog sudoku */
    char help_str[7] = "help: ";
    sprintf(help_str + strlen(help_str), "%d" , help_number);
    draw_text(help_str, 10, 25);

    /* ispis proteklog vremena */
    draw_elapsed_time(start_time);

    glutSwapBuffers();
}
