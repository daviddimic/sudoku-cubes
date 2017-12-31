#include <GL/glut.h>

#include "keyboard.h"
#include "display.h"


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sudoku kocka");

    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(on_specialkeys);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    initialize();

    glutMainLoop();

    return 0;
}
