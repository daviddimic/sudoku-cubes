#ifndef SUDOKU_DRAW_H_
#define  SUDOKU_DRAW_H_

/* Iscrtava se cifra od 0 do 9 sa datim koordinatama i velicinom*/
void draw_digit(int number, double x, double y, double size);

/* Iscrtava se tabla za igru sa donjim desnim uglom u x, y i sirinom width*/
void draw_table(double x, double y, double width);

#endif /*SUDOKU_DRAW_H_*/
