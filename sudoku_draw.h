#ifndef SUDOKU_DRAW_H_
#define  SUDOKU_DRAW_H_

#define N 9

/* Iscrtava se cifra od 0 do 9 sa datim koordinatama i velicinom*/
void draw_digit(const int number, double x, double y, double size);

/* Iscrtava se tabla (mreza) za igru sa donjim desnim uglom u x, y i sirinom width*/
void draw_grid(double x, double y, double width);

/* Iscrtava sve brojeve prilagodjene tabli sirine table_width sa koordinatama u donjim desnim uglom x, y
    indx i indy je tekuce oznaceno polje */
void draw_numbers(const int table[][N], int indx, int indy, int border, double x, double y, double table_width, double r, double g, double b);

/* Iscrtava se tabla: brojevi i mreza */
void draw_table(const int table[][N], int indx, int indy, int border, double x, double y, double width, double r, double g, double b);

#endif /*SUDOKU_DRAW_H_*/
