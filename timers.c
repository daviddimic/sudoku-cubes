#include "timers.h"
#include "sudoku_cube.h"

#include <GL/glut.h>
#include <assert.h>

/* externe promenljive */
int timer_rotate_active = 0;
int timer_move_active = 0;
/* za tajmer okretanja kocke on_timer_wsad */
char unsigned wsad_key;
/* tekuca tabla */
int curr_table;
/* rotacija kocke */
float x_rotation, y_rotation;
/* pozicija kamera */
float camera_x, camera_y, camera_z;
/* parametri kretanaj kocke */
float x_t, y_t;

/* vreme animacije - za kretanje kocke */
static float t;
/*TODO rand menjati pocetna brzina kretanja kocke */
static const float v0 = 6;


/* pomocna f-ja: za koliko treba rotirati kocku i kada zaustaviti tajmer */
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


/* tajmer za okretanje kocke */
void on_timer_wsad(int value) {

    if(value != TIMER_ROTATE_ID) return;

    assert( wsad_key == 'w' ||
            wsad_key == 's' ||
            wsad_key == 'a' ||
            wsad_key == 'd');

    switch (wsad_key) {
        case 'w':
            /* ne moze se ici napred ako smo na gornjoj tabli */
            if(curr_table == BACK)
                curr_table = UP;
            else
                x_rotation += 10;

            set_rotation_and_timer(&x_rotation, &timer_rotate_active);
        break;

        case 's':
            /* ne moze se ici dole ako smo na donjoj tabli */
            if(curr_table == BACK)
                curr_table = DOWN;
            else
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


/* TODO izmeniti tajmer za kretanje kocke */
void on_timer_move(int value) {

    if(value != TIMER_MOVE_ID) return;

    t += 0.05;

    x_t = v0*t*t;
    y_t = -t*t/(v0-2);

    // da upadne u kocku
    if(IN(x_t - 7 - camera_x, -EPS, EPS) && IN(y_t + 1 - camera_y, - EPS, EPS)){
        timer_move_active = 0;
    }

    glutPostRedisplay();

    if(timer_move_active){
        glutTimerFunc(TIMER_MOVE_WAIT, on_timer_move, TIMER_MOVE_ID);
    }
}
