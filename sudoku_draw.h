#ifndef SUDOKU_DRAW_H_
#define  SUDOKU_DRAW_H_

/* Iscrtava se cifra od 0 do 9 sa datim koordinatama i velicinom*/
void draw_digit(int number, double x, double y, double size);

/* Iscrtava se tabla za igru sa donjim desnim uglom u x, y i sirinom width*/
void draw_grid(double x, double y, double width);

/* Iscrtava sve brojeve prilagodjene tabli sirine table_width sa koordinatama u donjim desnim uglom x, y
    indx i indy je tekuce oznaceno polje */
void draw_numbers(int table[][9], int indx, int indy, double x, double y, double table_width);

#endif /*SUDOKU_DRAW_H_*/
