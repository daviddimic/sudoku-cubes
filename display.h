#ifndef DISPLAY_H_
#define DISPLAY_H_


void initialize(void);
void on_display(void);
void on_reshape(int width, int height);
void on_mouse_move(int x, int y);
void on_mouse(int button, int state, int x, int y);

#endif /* DISPLAY_H_ */
