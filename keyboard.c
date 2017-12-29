#include "keyboard.h"
#include "sudoku_cube.h"
#include "sudoku_func.h"
#include "timers.h"


#include <GL/glut.h>
#include <stdlib.h>

T tables[NUM_TABLES];
int curr_table;
int help_number;
double zoomInOut;
unsigned char wsad_key;

void on_specialkeys(int key, int x, int y){
    switch (key) {

    case GLUT_KEY_UP:
        tables[curr_table].indy =
            tables[curr_table].indy <= 0 ? 0 : tables[curr_table].indy - 1;
        break;
    case GLUT_KEY_DOWN:
        tables[curr_table].indy =
            tables[curr_table].indy >= N-1 ? N-1 : tables[curr_table].indy + 1;
        break;
    case GLUT_KEY_LEFT:
        tables[curr_table].indx =
            tables[curr_table].indx <= 0 ? 0 : tables[curr_table].indx - 1;
        break;
    case GLUT_KEY_RIGHT:
        tables[curr_table].indx =
            tables[curr_table].indx >= N-1 ? N-1 : tables[curr_table].indx + 1;
        break;
    }

    glutPostRedisplay();
}

void on_keyboard(unsigned char key, int x, int y) {
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
            if (!timer_rotate_active) {
                wsad_key = key;
                curr_table = next_table(key, curr_table);
                glutTimerFunc(TIMER_ROTATE_WAIT, on_timer_wsad, TIMER_ROTATE_ID);
                timer_rotate_active = 1;
            }
        break;

        /* pomeranje kamere, zoomIn, zoomOut */
        case '+': zoomInOut = zoomInOut <= -3.5 ? zoomInOut : zoomInOut - 0.1; break;
        case '-': zoomInOut = zoomInOut >= 1.5 ? zoomInOut : zoomInOut + 0.1;  break;

        /* TODO pokretanje kretanaj kocke
        case ' ':
            if (!timer_active_fly) {
                glutTimerFunc(20, on_timer_move, 1);
                timer_active_fly = 1;
            }
        break;
        */

        /* unos brojeva u tekucu tablu */
        case '0': case '1': case '2':
        case '3': case '4': case '5':
        case '6': case '7': case '8':
        case '9':
        number = key - '0';
        /* ne moze se promeniti polje iz originalne postavke */
        if(!tables[curr_table].original[tables[curr_table].indy][tables[curr_table].indx]){
            /* ne moze se upisati broj osim nule ako dodje do konflikta brojeva */
            if(!(number &&
                is_conflict(tables[curr_table].user, N, number, tables[curr_table].indy, tables[curr_table].indx))){
                tables[curr_table].user[tables[curr_table].indy][tables[curr_table].indx] = number;
            }
        }
        break;
    }
    glutPostRedisplay();
}