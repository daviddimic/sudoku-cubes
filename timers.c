#include "timers.h"
#include "sudoku_cube.h"

#include <GL/glut.h>
#include <assert.h>

int timer_rotate_active = 0;
char unsigned wsad_key;
int curr_table;
float x_rotation, y_rotation;

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
