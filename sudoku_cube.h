#ifndef SUDOKU_CUBE_H_
#define  SUDOKU_CUBE_H_

/* broj tabli na kocki */
#define NUM_TABLES 6
#define N 9

enum position {
    FRONT,
    BACK,
    RIGHT,
    LEFT,
    DOWN,
    UP
};

/*  jedan sudoku mora cuvati tekuci koji korisnik resava i originalnu postavku
 *  kao i poziciju na tabli gde se upisuje broj
*/
typedef struct {
    /*korisnicka tabla*/
    int user[N][N];
    /*originalna postavka table*/
    int original[N][N];
    /*indeks pozicije na tekucoj tabli*/
    int indx, indy;
} T;

/* ucitava se sudoku kocka - niz tabli */
void init_tables(T tables[NUM_TABLES], int n_tables, int n_table);

/* vraca sledecu poziciju table u zavisnosti od tastera w-s-a-d */
enum position next_table(unsigned char key, enum position curr_table);

/* crta sudoku kocku u (0,0,0) */
void draw_cube(T tables[NUM_TABLES], double size, int curr_table);

/* iscrtava 2D tekst na poziciji (x,y)*/
void draw_text(const char* text, double x, double y);

#endif /*SUDOKU_CUBE_H_*/
