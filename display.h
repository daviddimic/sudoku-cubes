#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "sudoku_cube.h"

/* tekuca tabla koja se resava */
extern int curr_table;
/* niz tabli */
extern T tables[NUM_TABLES];
/* broj pomoci za resavanje sudoku */
extern int help_number;

/* uglovi rotacije kocke */
extern float x_rotation;
extern float y_rotation;

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


void initialize(void);
void on_display(void);
void on_reshape(int width, int height);

#endif /* DISPLAY_H_ */
