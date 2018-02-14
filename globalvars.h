#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#include "sudoku_cube.h"


/* niz tabli */
extern T tables[NUM_TABLES];
/* tekuca tabla koja se resava */
extern int curr_table;
/* broj pomoci za resavanje sudoku */
extern int help_number;

/* rotacije kocke - okretanje na wsad */
extern float x_rotation;
extern float y_rotation;

/* ugao i translacija spinovanja kocke */
extern float spin_angle;
extern float spin_y;

/* skakanje posmatraca (kamere) */
extern float jump;

/* pocetne koordinate kamere */
extern float camera_x, camera_y, camera_z;
/* priblizavanje i udaljavanje pogleda na kocku */
extern double zoomInOut;

/* pocetna pozicija kocke (za move) */
extern float cube_start_x, cube_start_z;

/* parametri kretanja kocke */
extern float x_t, z_t;

/* vreme pri pokretanju programa */
extern time_t start_time;

extern float window_width, window_height;


#endif /* GLOBALVARS_H_ */
