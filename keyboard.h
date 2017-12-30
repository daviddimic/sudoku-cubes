#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define UNUSED(x) ((void)(x))

/* sa strelicama se krece kroz tekucu sudoku tablu */
void on_specialkeys(int key, int x, int y);

void on_keyboard(unsigned char key, int x, int y);


#endif /* KEYBOARD_H_ */
