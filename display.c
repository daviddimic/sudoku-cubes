#include "display.h"
#include "globalvars.h"
#include "keyboard.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <GL/glut.h>


static const char* usage_str = " rotate cube: w-s-a-d\n\
navigate in table: arrow keys\n\
zoom in/out: + -\n\
input number: 1-9\n\
remove number: 0\n\
remove all numbers: q\n\
solve one sudoku: h\n\
new game: n\n\
jump: space";


void initialize(void) {
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

    /* inicijalizacija globalnih promenljivih */

    /* pocetni polozaj kocke, u II kvadrantu xz ravni */
    srand(time(NULL));
    cube_start_x = -(rand()%40 + 10);
    cube_start_z = -(rand()%40 + 10);

    x_t = cube_start_x;
    z_t = cube_start_z;

    camera_x = 1;
    camera_y = 0.8;
    camera_z = 1.5;

    zoomInOut = 0;

    x_rotation = 0;
    y_rotation = 0;

    spin_angle = 0;
    spin_y = 0;

    jump = 0;

    /* ucitavamo 6 tabli */
    init_tables(tables, NUM_TABLES, N, &help_number, &start_time);
    /* pocetna tabla je prednja */
    curr_table = FRONT;

    /* odmah se pokrece animacija
     * kocke sa rand pozicije gadjaju kameru
    */
    if (!timer_move_active) {
        glutTimerFunc(TIMER_MOVE_WAIT, on_timer_move, TIMER_MOVE_ID);
        timer_move_active = 1;
    }
}

static void draw_all_msg(void){
    /* poruka o koriscenju */
    draw_text(usage_str, 10, 780);

    /* ispis poruke o broju pomoci - resavanje jednog sudoku */
    char help_str[7] = "help: ";
    sprintf(help_str + strlen(help_str), "%d" , help_number);
    draw_text(help_str, 10, 25);

    /* ispis proteklog vremena */
    draw_elapsed_time(start_time);
}

void on_display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* postavljanej pozicije svetla */
    GLfloat light_position[] = {-0.5, 1, 4, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glShadeModel(GL_SMOOTH);

    /* podesava se tacka pogleda */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            camera_x + zoomInOut, camera_y + zoomInOut + jump, camera_z,
            0, jump, 0,
            0, 1, 0);

    /* velicina kocke */
    double size = 1;

    /* kretanje kocke ka posmatracu */
    glTranslatef(x_t, 0, z_t);
    glRotatef(10*x_t, 1, 1, 1);

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

    /* ispis celog 2D teksta */
    draw_all_msg();

    glutSwapBuffers();
}

void on_reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 0.2, 40);
}
