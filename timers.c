#include "timers.h"
#include "globalvars.h"
#include "sudoku_cube.h"

#include <GL/glut.h>
#include <assert.h>

/* globalne promenljive
 * inicijalizacija aktivnosti tajmera
 */
int timer_rotate_active = 0;
int timer_move_active = 0;
int timer_spin_active = 0;
int timer_jump_active = 0;


/* vreme kretanja kocke */
static float move_t = 0;
/* vreme spinovanja kocke */
static float spin_t = 0.02;
/* vreme skakanja */
static float jump_t = 0;


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

    /* NOTE
     * curr_table ovde treba shvatiti kao SLEDECA tabla
     * jer kada se registruje neki od tastera w, s, a, d
     * prvo se poziva next_table pa onda rotira kocka
     */

    switch (wsad_key) {
        case 'w':
            /* ne moze se ici napred ako smo na gornjoj tabli */
            if(curr_table == BACK)
                curr_table = UP;
            else /* ne moze se ici gore ako smo ga donjoj tabli koja je rotirana */
                if(curr_table != UP && curr_table != DOWN && y_rotation)
                    curr_table = DOWN;
                else
                    x_rotation += 10;

            set_rotation_and_timer(&x_rotation, &timer_rotate_active);
        break;

        case 's':
            /* ne moze se ici dole ako smo na donjoj tabli */
            if(curr_table == BACK)
                curr_table = DOWN;
            else /* ne moze se ici dole ako smo ga gornjoj tabli koja je rotirana */
                if(curr_table != UP && curr_table != DOWN && y_rotation)
                    curr_table = UP;
                else /* rotacija se vrsi tek ako ova dva uslova ne prodju */
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

void spin_timer(int value) {
    if(value != TIMER_SPIN_ID) return;

    spin_t += .02;

    /* rotacija */
    spin_angle =  360*(2*v0spin/G)/(spin_t + 0.0001);
    /*translacija - jednacina hitac u vis */
    spin_y = v0spin*spin_t - G*spin_t*spin_t/2;

    /* kada objekat padne na zemlju zaustavlja se animacija */
    if( (spin_t - 2*v0spin/G >= -0.01) && (spin_t - 2*v0spin/G <= 0.01) ){
        spin_t = 0.02;
        spin_y = 0;
        spin_angle = 0;
        timer_spin_active = 0;
    }

    glutPostRedisplay();

    if(timer_spin_active){
        glutTimerFunc(TIMER_SPIN_WAIT, spin_timer, TIMER_SPIN_ID);
    }
}

void on_timer_jump(int value){
    if(value != TIMER_JUMP_ID) return;

    jump_t += .02;

    /*translacija - jednacina hitac u vis */
    jump = v0jump*jump_t - G*jump_t*jump_t/2;
    /* kada padne na zemlju zaustavlja se animacija */
    if( (jump_t - 2*v0jump/G >= -0.01) && (jump_t - 2*v0jump/G <= 0.01) ){
        jump_t = 0;
        jump = 0;
        timer_jump_active = 0;
    }

    glutPostRedisplay();

    if(timer_jump_active){
        glutTimerFunc(TIMER_JUMP_WAIT, on_timer_jump, TIMER_JUMP_ID);
    }
}

/* parametarska jednacina duzi */
static void lineAB(float Ax, float Az, float Bx, float Bz, float t){
    x_t = Ax + t*(Bx - Ax);
    z_t = Az + t*(Bz - Az);
}


void on_timer_move(int value) {

    if(value != TIMER_MOVE_ID) return;

    /* polozaj kamere dok se kocke krecu */
    camera_x = 1;
    camera_y = 0.2;
    camera_z = 1;

    move_t += 0.03;

    /* duz od rand tacke do kamere */
    lineAB(cube_start_x, cube_start_z, camera_x, camera_z, move_t);

    if(move_t >= 0.99 && move_t <= 1.05) {
        if(cube_in_camera(jump, camera_y, 1)){
            /* ako je pogodjena kamera kocka se stavlja u (0,0) i resava se kocka */
            /* inicijalizuje se nova kocka */
            init_tables(tables, NUM_TABLES, N, &help_number, &start_time);
            move_t = 0;
            x_t = 0;
            z_t = 0;
            camera_x = 1;
            camera_y = 0.8;
            camera_z = 1.5;
            timer_move_active = 0;
        }
    }
    /* ako nije pogodjena kocka sa rand pozicije ponovo gadja
     * parametar move_t ide duplo duze da uspori prebrzo generisanje kocki
     */
    else if(move_t >= 2) {
       srand(time(NULL));
       /* pocetni polozaj kocke */
       cube_start_x = -(rand()%40 + 10);
       cube_start_z = -(rand()%40 + 10);
       move_t = 0;
    }

    glutPostRedisplay();

    if(timer_move_active){
        glutTimerFunc(TIMER_MOVE_WAIT, on_timer_move, TIMER_MOVE_ID);
    }
}
